#ifndef RADIOSTATE_H
#define RADIOSTATE_H

#include <QObject>
#include <QString>

class AudioController;
class SignalingClient;

/**
 * Автомат состояний и правила эфира PTT-рации.
 *
 * IDLE         — канал не активен, сетевые и аудиоресурсы освобождены.
 * LISTENING    — канал активен; слушаем эфир или ждём подтверждения PTT.
 * TRANSMITTING — сервер выдал нам эфир, идёт передача.
 *
 * Класс не зависит от QML. Интерфейс вызывает публичные команды и читает
 * Q_PROPERTY, а сетевые ответы поступают через SignalingClient.
 */
class RadioState : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Mode mode READ mode NOTIFY modeChanged)
    Q_PROPERTY(QString modeName READ modeName NOTIFY stateChanged)
    Q_PROPERTY(QString statusText READ statusText NOTIFY stateChanged)
    Q_PROPERTY(QString channel READ channel NOTIFY stateChanged)
    Q_PROPERTY(QString speakerName READ speakerName NOTIFY speakerChanged)
    Q_PROPERTY(bool simulationMode READ simulationMode WRITE setSimulationMode NOTIFY stateChanged)
    Q_PROPERTY(bool applicationActive READ applicationActive NOTIFY stateChanged)
    Q_PROPERTY(bool pttPending READ pttPending NOTIFY stateChanged)
    Q_PROPERTY(bool joinPending READ joinPending NOTIFY stateChanged)
    Q_PROPERTY(bool canTalk READ canTalk NOTIFY stateChanged)
    Q_PROPERTY(bool remoteSpeaker READ remoteSpeaker NOTIFY stateChanged)
    Q_PROPERTY(bool keepAlive READ keepAlive NOTIFY stateChanged)

public:
    enum Mode {
        Idle,
        Listening,
        Transmitting
    };
    Q_ENUM(Mode)

    explicit RadioState(SignalingClient *client, AudioController *audio,
                        QObject *parent = nullptr);
    ~RadioState() override;

    Mode mode() const { return m_mode; }
    QString modeName() const;
    QString statusText() const;
    QString channel() const { return m_channel; }
    QString speakerName() const { return m_speakerName; }
    bool simulationMode() const { return m_simulationMode; }
    bool applicationActive() const { return m_applicationActive; }
    bool pttPending() const { return m_pttPending; }
    bool joinPending() const { return m_joinPending; }
    bool canTalk() const;
    bool remoteSpeaker() const;
    bool keepAlive() const;

    Q_INVOKABLE void connectToChannel(const QString &host, quint16 port,
                                      const QString &channel, const QString &myName);
    Q_INVOKABLE void simulateIncomingTransmission();

public slots:
    void joinChannel(const QString &channel, const QString &myName);
    void leaveChannel();
    void requestTalk();
    void releaseTalk();
    void setSimulationMode(bool enabled);
    void setApplicationActive(bool active);
    void onServerMessage(const QString &type, const QString &data);

signals:
    void modeChanged(Mode mode);
    void speakerChanged(const QString &name);
    void stateChanged();
    void logEntry(const QString &text);

private slots:
    void onConnected();
    void onDisconnected();
    void onNetworkError(const QString &errorText);

private:
    void setMode(Mode newMode);
    void setSpeaker(const QString &name);
    void setPttPending(bool pending);
    void setJoinPending(bool pending);
    void sendJoin();
    void resetSession(bool clearChannel);
    void syncAudio();

    SignalingClient *m_client;
    AudioController *m_audio;
    Mode m_mode = Idle;
    QString m_channel;
    QString m_myName;
    QString m_speakerName;
    QString m_host;
    quint16 m_port = 5000;
    bool m_simulationMode = true;
    bool m_applicationActive = true;
    bool m_pttPending = false;
    bool m_joinPending = false;
    bool m_cancelPtt = false;
};

#endif // RADIOSTATE_H
