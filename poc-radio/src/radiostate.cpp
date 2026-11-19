#include "radiostate.h"
#include "audiocontroller.h"
#include "signalingclient.h"

namespace {
bool hasWhitespace(const QString &text)
{
    for (int i = 0; i < text.size(); ++i) {
        if (text.at(i).isSpace())
            return true;
    }
    return false;
}
}

RadioState::RadioState(SignalingClient *client, AudioController *audio, QObject *parent)
    : QObject(parent)
    , m_client(client)
    , m_audio(audio)
{
    connect(m_client, &SignalingClient::connected, this, &RadioState::onConnected);
    connect(m_client, &SignalingClient::disconnected, this, &RadioState::onDisconnected);
    connect(m_client, &SignalingClient::errorOccurred, this, &RadioState::onNetworkError);
    connect(m_client, &SignalingClient::parsedMessage, this, &RadioState::onServerMessage);
    connect(m_audio, &AudioController::logEntry, this, &RadioState::logEntry);
}

RadioState::~RadioState()
{
    m_audio->stopAll();
}

QString RadioState::modeName() const
{
    switch (m_mode) {
    case Idle:
        return QStringLiteral("IDLE");
    case Listening:
        return QStringLiteral("LISTENING");
    case Transmitting:
        return QStringLiteral("TRANSMITTING");
    }
    return QStringLiteral("IDLE");
}

QString RadioState::statusText() const
{
    if (m_joinPending)
        return QStringLiteral("Подключение и вход в канал…");

    switch (m_mode) {
    case Idle:
        return QStringLiteral("Вне канала. Аппаратные ресурсы освобождены.");
    case Listening:
        if (m_pttPending)
            return QStringLiteral("Запрашиваю право говорить…");
        if (remoteSpeaker())
            return QStringLiteral("Говорит %1. Передача заблокирована.").arg(m_speakerName);
        if (!m_speakerName.isEmpty() && m_speakerName == m_myName)
            return QStringLiteral("Ожидаю подтверждение сервера…");
        return QStringLiteral("Канал «%1» свободен. Можно говорить.").arg(m_channel);
    case Transmitting:
        return QStringLiteral("Эфир ваш. Говорите, удерживая PTT.");
    }
    return QString();
}

bool RadioState::canTalk() const
{
    return m_applicationActive && m_mode == Listening && m_speakerName.isEmpty()
            && !m_pttPending && !m_cancelPtt;
}

bool RadioState::remoteSpeaker() const
{
    return m_mode != Idle && !m_speakerName.isEmpty() && m_speakerName != m_myName;
}

bool RadioState::keepAlive() const
{
    // Не расходуем батарею в пассивном IDLE/LISTENING без говорящего.
    return m_mode == Transmitting || remoteSpeaker();
}

void RadioState::setSimulationMode(bool enabled)
{
    if (m_mode != Idle || m_joinPending) {
        emit logEntry(QStringLiteral("Сначала выйдите из канала, затем меняйте режим"));
        return;
    }
    if (m_simulationMode == enabled)
        return;

    m_simulationMode = enabled;
    emit stateChanged();
}

void RadioState::setApplicationActive(bool active)
{
    if (m_applicationActive == active)
        return;

    m_applicationActive = active;

    // Сворачивание не должно оставлять включённую передачу или занятый эфир.
    if (!m_applicationActive && (m_mode == Transmitting || m_pttPending))
        releaseTalk();

    syncAudio();
    emit stateChanged();
}

void RadioState::connectToChannel(const QString &host, quint16 port,
                                  const QString &channel, const QString &myName)
{
    if (m_simulationMode) {
        joinChannel(channel, myName);
        return;
    }

    if (host.trimmed().isEmpty() || port == 0) {
        emit logEntry(QStringLiteral("Укажите адрес сервера и корректный порт"));
        return;
    }
    if (channel.trimmed().isEmpty() || myName.trimmed().isEmpty()) {
        emit logEntry(QStringLiteral("Укажите канал и имя перед входом"));
        return;
    }
    if (hasWhitespace(channel)) {
        emit logEntry(QStringLiteral("Имя канала не должно содержать пробелов: протокол JOIN их не экранирует"));
        return;
    }
    if (myName.contains(QLatin1Char('\n')) || myName.contains(QLatin1Char('\r'))) {
        emit logEntry(QStringLiteral("Имя участника не должно содержать перевод строки"));
        return;
    }

    m_client->disconnectFromServer();
    resetSession(false);
    m_host = host.trimmed();
    m_port = port;
    m_channel = channel.trimmed();
    m_myName = myName.trimmed();

    m_client->connectToServer(m_host, m_port);
    setJoinPending(true);
    emit logEntry(QStringLiteral("Подключаюсь к %1:%2…").arg(m_host).arg(m_port));
}

void RadioState::joinChannel(const QString &channel, const QString &myName)
{
    if (m_mode != Idle || m_joinPending) {
        emit logEntry(QStringLiteral("Сначала завершите текущий сеанс"));
        return;
    }
    if (channel.trimmed().isEmpty() || myName.trimmed().isEmpty()) {
        emit logEntry(QStringLiteral("Укажите канал и имя перед входом"));
        return;
    }
    if (hasWhitespace(channel)) {
        emit logEntry(QStringLiteral("Имя канала не должно содержать пробелов: протокол JOIN их не экранирует"));
        return;
    }
    if (myName.contains(QLatin1Char('\n')) || myName.contains(QLatin1Char('\r'))) {
        emit logEntry(QStringLiteral("Имя участника не должно содержать перевод строки"));
        return;
    }

    m_channel = channel.trimmed();
    m_myName = myName.trimmed();

    if (m_simulationMode) {
        emit logEntry(QStringLiteral("Локальная симуляция: вход в канал «%1»").arg(m_channel));
        setMode(Listening);
        return;
    }

    if (!m_client->isConnected()) {
        emit logEntry(QStringLiteral("Нет соединения: используйте кнопку входа с адресом сервера"));
        m_channel.clear();
        emit stateChanged();
        return;
    }

    setJoinPending(true);
    sendJoin();
}

void RadioState::leaveChannel()
{
    if (m_mode == Idle && !m_joinPending)
        return;

    if (!m_simulationMode && m_client->isConnected())
        m_client->sendMessage(QStringLiteral("LEAVE"));

    m_client->disconnectFromServer();
    resetSession(true);
    emit logEntry(QStringLiteral("Канал закрыт, ресурсы освобождены"));
}

void RadioState::requestTalk()
{
    if (!m_applicationActive) {
        emit logEntry(QStringLiteral("Передача возможна только при открытом приложении"));
        return;
    }
    if (m_mode != Listening) {
        emit logEntry(QStringLiteral("Сначала войдите в канал"));
        return;
    }
    if (m_pttPending)
        return;
    if (!m_speakerName.isEmpty()) {
        emit logEntry(QStringLiteral("Эфир занят: говорит %1").arg(m_speakerName));
        return;
    }

    if (m_cancelPtt) {
        emit logEntry(QStringLiteral("Дождитесь завершения предыдущей отмены PTT"));
        return;
    }

    if (m_simulationMode) {
        // Режим включаем раньше установки speaker: QML не должен терять
        // удержание кнопки в промежуточном состоянии.
        setMode(Transmitting);
        setSpeaker(m_myName);
        emit logEntry(QStringLiteral("Симуляция: эфир выдан локально"));
        return;
    }

    m_cancelPtt = false;

    // TRANSMITTING включается только после ACK PTT_GRANTED от сервера.
    setPttPending(true);
    m_client->sendMessage(QStringLiteral("PTT_START"));
    emit logEntry(QStringLiteral("Запрошен эфир (PTT_START)"));
}

void RadioState::releaseTalk()
{
    const bool wasPending = m_pttPending;
    if (!wasPending && m_mode != Transmitting)
        return;

    if (m_simulationMode) {
        setPttPending(false);
        if (m_speakerName == m_myName)
            setSpeaker(QString());
        setMode(Listening);
        emit logEntry(QStringLiteral("Передача завершена, эфир свободен"));
        return;
    }

    if (wasPending) {
        // Если подтверждение придёт позже, onServerMessage повторит PTT_STOP.
        m_cancelPtt = true;
        setPttPending(false);
        m_client->sendMessage(QStringLiteral("PTT_STOP"));
        emit logEntry(QStringLiteral("Запрос эфира отменён"));
        return;
    }

    m_client->sendMessage(QStringLiteral("PTT_STOP"));
    m_cancelPtt = false;
    if (m_speakerName == m_myName)
        setSpeaker(QString());
    setMode(Listening);
    emit logEntry(QStringLiteral("Эфир освобождён (PTT_STOP)"));
}

void RadioState::simulateIncomingTransmission()
{
    if (!m_simulationMode || m_mode == Idle)
        return;

    if (remoteSpeaker()) {
        setSpeaker(QString());
        emit logEntry(QStringLiteral("Симуляция: входящая передача завершена"));
        return;
    }

    if (m_mode == Listening && m_speakerName.isEmpty() && !m_pttPending) {
        setSpeaker(QStringLiteral("Другой участник"));
        emit logEntry(QStringLiteral("Симуляция: другой участник занял эфир"));
    }
}

void RadioState::onServerMessage(const QString &type, const QString &data)
{
    if (m_simulationMode)
        return;

    if (type == QLatin1String("ACK")) {
        if (data == QLatin1String("JOINED")) {
            if (!m_joinPending)
                return;
            setJoinPending(false);
            setMode(Listening);
            emit logEntry(QStringLiteral("Вход выполнен. Слушаю канал «%1»").arg(m_channel));
        } else if (data == QLatin1String("PTT_GRANTED")) {
            if (m_mode == Transmitting)
                return; // повторное подтверждение

            if (m_cancelPtt || !m_pttPending || !m_applicationActive || m_mode != Listening) {
                m_client->sendMessage(QStringLiteral("PTT_STOP"));
                // До SPEAKER (пусто) отмена остаётся активной: старый ACK
                // не должен разрешить немедленный повторный PTT.
                setPttPending(false);
                emit logEntry(QStringLiteral("Позднее подтверждение эфира отклонено"));
                return;
            }

            m_cancelPtt = false;
            setPttPending(false);
            // Сначала режим, затем speakerName: иначе QML на мгновение
            // отключает MouseArea во время удержания PTT.
            setMode(Transmitting);
            setSpeaker(m_myName);
            emit logEntry(QStringLiteral("Сервер выдал эфир — можно говорить"));
        } else if (data == QLatin1String("PTT_RELEASED")) {
            m_cancelPtt = false;
            if (m_speakerName == m_myName)
                setSpeaker(QString());
            if (m_mode == Transmitting)
                setMode(Listening);
            emit logEntry(QStringLiteral("Сервер подтвердил освобождение эфира"));
        } else if (data == QLatin1String("LEFT")) {
            resetSession(true);
        }
    } else if (type == QLatin1String("SPEAKER")) {
        if (data.isEmpty()) {
            setSpeaker(QString());
            m_cancelPtt = false;
            if (m_mode == Transmitting) {
                setMode(Listening);
                emit logEntry(QStringLiteral("Сервер завершил передачу"));
            }
            emit stateChanged();
        } else if (data == m_myName) {
            if (m_cancelPtt || !m_applicationActive) {
                m_client->sendMessage(QStringLiteral("PTT_STOP"));
            } else if (m_mode == Transmitting) {
                setSpeaker(data);
            }
        } else {
            if (m_mode == Transmitting) {
                // Защитное правило: даже при ошибке сервера не передаём вдвоём.
                m_client->sendMessage(QStringLiteral("PTT_STOP"));
                setMode(Listening);
            }
            setSpeaker(data);
            emit logEntry(QStringLiteral("Говорит %1").arg(data));
        }
    } else if (type == QLatin1String("ERROR")) {
        setPttPending(false);
        m_cancelPtt = false;
        if (data.startsWith(QLatin1String("BUSY"))) {
            const QString busySpeaker = data.mid(4).trimmed();
            if (!busySpeaker.isEmpty())
                setSpeaker(busySpeaker);
            emit logEntry(QStringLiteral("Сервер отклонил PTT: эфир занят (%1)")
                          .arg(busySpeaker.isEmpty() ? QStringLiteral("другой участник") : busySpeaker));
        } else {
            if (data == QLatin1String("NOT_IN_CHANNEL"))
                resetSession(false);
            emit logEntry(QStringLiteral("Ошибка сервера: %1").arg(data));
        }
    } else if (type == QLatin1String("USERS")) {
        emit logEntry(QStringLiteral("Участники: %1").arg(data));
    }
}

void RadioState::onConnected()
{
    emit logEntry(QStringLiteral("TCP-соединение установлено"));
    if (m_joinPending)
        sendJoin();
}

void RadioState::onDisconnected()
{
    if (m_simulationMode)
        return;

    const bool hadSession = m_mode != Idle || m_joinPending;
    resetSession(false);
    if (hadSession)
        emit logEntry(QStringLiteral("Соединение разорвано. Канал можно открыть повторно."));
}

void RadioState::onNetworkError(const QString &errorText)
{
    if (m_joinPending)
        setJoinPending(false);
    emit logEntry(QStringLiteral("Сеть: %1").arg(errorText));
}

void RadioState::sendJoin()
{
    m_client->sendMessage(QStringLiteral("JOIN %1 %2").arg(m_channel, m_myName));
    emit logEntry(QStringLiteral("Вхожу в канал «%1» как «%2»").arg(m_channel, m_myName));
}

void RadioState::setMode(Mode newMode)
{
    if (m_mode == newMode)
        return;

    m_mode = newMode;
    syncAudio();
    emit modeChanged(m_mode);
    emit stateChanged();
}

void RadioState::setSpeaker(const QString &name)
{
    if (m_speakerName == name)
        return;

    m_speakerName = name;
    syncAudio();
    emit speakerChanged(m_speakerName);
    emit stateChanged();
}

void RadioState::setPttPending(bool pending)
{
    if (m_pttPending == pending)
        return;

    m_pttPending = pending;
    emit stateChanged();
}

void RadioState::setJoinPending(bool pending)
{
    if (m_joinPending == pending)
        return;

    m_joinPending = pending;
    emit stateChanged();
}

void RadioState::resetSession(bool clearChannel)
{
    const Mode oldMode = m_mode;
    const QString oldSpeaker = m_speakerName;

    m_audio->stopAll();
    m_mode = Idle;
    m_speakerName.clear();
    m_pttPending = false;
    m_joinPending = false;
    m_cancelPtt = false;
    if (clearChannel)
        m_channel.clear();

    if (oldMode != m_mode)
        emit modeChanged(m_mode);
    if (oldSpeaker != m_speakerName)
        emit speakerChanged(m_speakerName);
    emit stateChanged();
}

void RadioState::syncAudio()
{
    const bool shouldTransmit = m_mode == Transmitting && m_applicationActive;
    const bool shouldReceive = m_mode == Listening && remoteSpeaker();

    if (shouldTransmit)
        m_audio->startTransmitting();
    else
        m_audio->stopTransmitting();

    if (shouldReceive && !shouldTransmit)
        m_audio->startReceiving();
    else
        m_audio->stopReceiving();
}
