#include "signalingclient.h"

#include <QTcpSocket>

SignalingClient::SignalingClient(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this)) // 'this' — сокет удалится сам вместе с нами
{
    // connect(отправитель, его-сигнал, получатель, его-слот)
    // Теперь при приходе байтов автоматически вызовется onReadyRead().
    connect(m_socket, &QTcpSocket::readyRead, this, &SignalingClient::onReadyRead);
    connect(m_socket, &QTcpSocket::connected, this, &SignalingClient::onConnectedSlot);
    connect(m_socket, &QTcpSocket::disconnected, this, &SignalingClient::onDisconnectedSlot);
    // В Qt 5.6 сигнал называется error; errorOccurred появился позднее.
    connect(m_socket,
            static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(
                &QAbstractSocket::error),
            this,
            [this](QAbstractSocket::SocketError) {
                emit errorOccurred(m_socket->errorString());
            });
}

SignalingClient::~SignalingClient()
{
    disconnectFromServer();
}

bool SignalingClient::isConnected() const
{
    return m_socket->state() == QAbstractSocket::ConnectedState;
}

void SignalingClient::connectToServer(const QString &host, quint16 port)
{
    if (host.trimmed().isEmpty() || port == 0) {
        emit errorOccurred(QStringLiteral("Укажите адрес и порт сервера"));
        return;
    }
    if (isConnected())
        return;

    // Повторное подключение после ошибки или обрыва начинаем с чистого сокета.
    if (m_socket->state() != QAbstractSocket::UnconnectedState)
        m_socket->abort();
    m_rxBuffer.clear();
    m_socket->connectToHost(host.trimmed(), port);
}

void SignalingClient::disconnectFromServer()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState)
        m_socket->disconnectFromHost();
    else if (m_socket->state() != QAbstractSocket::UnconnectedState)
        m_socket->abort();
}

void SignalingClient::sendMessage(const QString &message)
{
    if (!isConnected()) {
        emit errorOccurred(QStringLiteral("Нет соединения с сервером"));
        return;
    }
    QByteArray data = message.toUtf8();
    data.append('\n'); // наш разделитель сообщений
    m_socket->write(data);
}

void SignalingClient::onConnectedSlot()
{
    emit connected();
}

void SignalingClient::onDisconnectedSlot()
{
    emit disconnected();
}

void SignalingClient::onReadyRead()
{
    // ВАЖНО: байты из сети могут приходить "кусками": одно сообщение
    // может прийти в два захода, а два сообщения — одним. Поэтому
    // копим входящие данные в буфере и отрезаем только полные строки.
    m_rxBuffer += m_socket->readAll();
    QByteArray &buffer = m_rxBuffer;

    while (buffer.contains('\n')) {
        const int idx = buffer.indexOf('\n');
        QByteArray line = buffer.left(idx);   // всё до переноса — целое сообщение
        buffer.remove(0, idx + 1);            // убираем его из буфера

        const QString text = QString::fromUtf8(line).trimmed();
        if (text.isEmpty())
            continue;
        emit messageReceived(text);

        // Разбираем: первое слово — тип, остальное — данные.
        const int space = text.indexOf(' ');
        const QString type = space == -1 ? text : text.left(space);
        const QString data = space == -1 ? QString() : text.mid(space + 1);
        emit parsedMessage(type, data);
    }
}
