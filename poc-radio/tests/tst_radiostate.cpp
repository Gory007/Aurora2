#include <QtTest>
#include <QCoreApplication>
#include <QElapsedTimer>
#include <QThread>
#include <QTcpServer>
#include <QTcpSocket>

#include "audiocontroller.h"
#include "radiostate.h"
#include "signalingclient.h"

class TestRadioState : public QObject
{
    Q_OBJECT

private slots:
    void startsIdle();
    void simulationPttCycle();
    void rejectsSecondSpeaker();
    void rejectsChannelWithWhitespace();
    void minimizeReleasesTransmission();
    void leaveReleasesResources();
    void realPttGrantAndRaceHandling();

private:
    static void processEvents(int milliseconds = 30)
    {
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < milliseconds) {
            QCoreApplication::processEvents(QEventLoop::AllEvents, 10);
            QThread::msleep(2);
        }
    }

    static bool waitForMode(RadioState &radio, RadioState::Mode mode, int timeout = 2000)
    {
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < timeout) {
            if (radio.mode() == mode)
                return true;
            processEvents(20);
        }
        return radio.mode() == mode;
    }

    static QString readLine(QTcpSocket *socket, int timeout = 2000)
    {
        QElapsedTimer timer;
        timer.start();
        while (timer.elapsed() < timeout) {
            if (socket->canReadLine())
                return QString::fromUtf8(socket->readLine()).trimmed();
            processEvents(20);
        }
        return QString();
    }

    static void sendLine(QTcpSocket *socket, const QString &line)
    {
        socket->write((line + QLatin1Char('\n')).toUtf8());
        socket->flush();
    }

    static QTcpSocket *joinRealRadio(QTcpServer &server, RadioState &radio)
    {
        radio.setSimulationMode(false);
        radio.connectToChannel(QHostAddress(QHostAddress::LocalHost).toString(), server.serverPort(),
                               QStringLiteral("test"), QStringLiteral("Ivan"));

        QElapsedTimer timer;
        timer.start();
        while (!server.hasPendingConnections() && timer.elapsed() < 2000)
            processEvents(20);
        if (!server.hasPendingConnections())
            return nullptr;

        QTcpSocket *serverSide = server.nextPendingConnection();
        if (readLine(serverSide) != QStringLiteral("JOIN test Ivan"))
            return nullptr;
        sendLine(serverSide, QStringLiteral("ACK JOINED"));
        if (!waitForMode(radio, RadioState::Listening))
            return nullptr;
        return serverSide;
    }
};

void TestRadioState::startsIdle()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);

    QCOMPARE(radio.mode(), RadioState::Idle);
    QCOMPARE(radio.modeName(), QStringLiteral("IDLE"));
    QVERIFY(!radio.canTalk());
    QVERIFY(!radio.keepAlive());
    QVERIFY(!audio.isTransmitting());
    QVERIFY(!audio.isReceiving());
}

void TestRadioState::simulationPttCycle()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);

    radio.joinChannel(QStringLiteral("test"), QStringLiteral("Ivan"));
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(radio.canTalk());

    radio.requestTalk();
    QCOMPARE(radio.mode(), RadioState::Transmitting);
    QCOMPARE(radio.speakerName(), QStringLiteral("Ivan"));
    QVERIFY(audio.isTransmitting());
    QVERIFY(radio.keepAlive());

    radio.releaseTalk();
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(radio.speakerName().isEmpty());
    QVERIFY(!audio.isTransmitting());
    QVERIFY(!radio.keepAlive());
}

void TestRadioState::rejectsSecondSpeaker()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);
    radio.joinChannel(QStringLiteral("test"), QStringLiteral("Ivan"));

    radio.simulateIncomingTransmission();
    QVERIFY(radio.remoteSpeaker());
    QVERIFY(audio.isReceiving());
    QVERIFY(!radio.canTalk());

    radio.requestTalk();
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(!audio.isTransmitting());

    radio.simulateIncomingTransmission();
    QVERIFY(!radio.remoteSpeaker());
    QVERIFY(!audio.isReceiving());
    QVERIFY(radio.canTalk());
}

void TestRadioState::rejectsChannelWithWhitespace()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);

    radio.joinChannel(QStringLiteral("room one"), QStringLiteral("Ivan"));
    QCOMPARE(radio.mode(), RadioState::Idle);
    QVERIFY(radio.channel().isEmpty());
}

void TestRadioState::minimizeReleasesTransmission()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);
    radio.joinChannel(QStringLiteral("test"), QStringLiteral("Ivan"));
    radio.requestTalk();
    QCOMPARE(radio.mode(), RadioState::Transmitting);

    radio.setApplicationActive(false);
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(radio.speakerName().isEmpty());
    QVERIFY(!audio.isTransmitting());
    QVERIFY(!radio.applicationActive());
    QVERIFY(!radio.canTalk());

    radio.setApplicationActive(true);
    QVERIFY(radio.canTalk());
}

void TestRadioState::leaveReleasesResources()
{
    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);
    radio.joinChannel(QStringLiteral("test"), QStringLiteral("Ivan"));
    radio.simulateIncomingTransmission();

    radio.leaveChannel();
    QCOMPARE(radio.mode(), RadioState::Idle);
    QVERIFY(radio.channel().isEmpty());
    QVERIFY(radio.speakerName().isEmpty());
    QVERIFY(!audio.isTransmitting());
    QVERIFY(!audio.isReceiving());
    QVERIFY(!radio.keepAlive());
}

void TestRadioState::realPttGrantAndRaceHandling()
{
    QTcpServer server;
    QVERIFY(server.listen(QHostAddress::LocalHost, 0));

    SignalingClient client;
    AudioController audio;
    RadioState radio(&client, &audio);
    QTcpSocket *serverSide = joinRealRadio(server, radio);
    QVERIFY(serverSide != nullptr);

    // До grant передача ещё не началась.
    radio.requestTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_START"));
    QVERIFY(radio.pttPending());
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(!audio.isTransmitting());

    sendLine(serverSide, QStringLiteral("ACK PTT_GRANTED"));
    QVERIFY(waitForMode(radio, RadioState::Transmitting));
    QVERIFY(audio.isTransmitting());

    radio.releaseTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_STOP"));
    QVERIFY(waitForMode(radio, RadioState::Listening));
    sendLine(serverSide, QStringLiteral("SPEAKER"));

    // Отпустили до grant: старое подтверждение не должно включить передачу.
    radio.requestTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_START"));
    radio.releaseTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_STOP"));
    QVERIFY(!radio.pttPending());

    sendLine(serverSide, QStringLiteral("ACK PTT_GRANTED"));
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_STOP"));
    processEvents(50);
    QCOMPARE(radio.mode(), RadioState::Listening);
    QVERIFY(!audio.isTransmitting());

    // Повторное нажатие до очистки отмены не создаёт новый запрос.
    radio.requestTalk();
    processEvents(50);
    QVERIFY(!radio.pttPending());
    QCOMPARE(serverSide->bytesAvailable(), qint64(0));

    sendLine(serverSide, QStringLiteral("SPEAKER"));
    processEvents(50);
    QVERIFY(radio.canTalk());

    // BUSY не включает передачу и показывает чужого говорящего.
    radio.requestTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_START"));
    sendLine(serverSide, QStringLiteral("ERROR BUSY Petr"));
    processEvents(50);
    QCOMPARE(radio.mode(), RadioState::Listening);
    QCOMPARE(radio.speakerName(), QStringLiteral("Petr"));
    QVERIFY(!audio.isTransmitting());

    sendLine(serverSide, QStringLiteral("SPEAKER"));
    processEvents(50);
    radio.requestTalk();
    QCOMPARE(readLine(serverSide), QStringLiteral("PTT_START"));
    sendLine(serverSide, QStringLiteral("ACK PTT_GRANTED"));
    QVERIFY(waitForMode(radio, RadioState::Transmitting));

    serverSide->disconnectFromHost();
    QVERIFY(waitForMode(radio, RadioState::Idle));
    QVERIFY(!audio.isTransmitting());
    serverSide->deleteLater();
}

QTEST_GUILESS_MAIN(TestRadioState)
#include "tst_radiostate.moc"
