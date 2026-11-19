#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include <QObject>

/**
 * Отдельная аудио-часть приложения.
 *
 * На этапе 1 здесь находятся только безопасные заглушки и состояния.
 * Захват микрофона, Opus, UDP и воспроизведение будут добавлены позднее,
 * без изменения QML и автомата состояний RadioState.
 */
class AudioController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool transmitting READ isTransmitting NOTIFY transmittingChanged)
    Q_PROPERTY(bool receiving READ isReceiving NOTIFY receivingChanged)

public:
    explicit AudioController(QObject *parent = nullptr);

    bool isTransmitting() const { return m_transmitting; }
    bool isReceiving() const { return m_receiving; }

public slots:
    void startTransmitting();
    void stopTransmitting();
    void startReceiving();
    void stopReceiving();
    void stopAll();

signals:
    void transmittingChanged(bool active);
    void receivingChanged(bool active);
    void logEntry(const QString &text);

private:
    bool m_transmitting = false;
    bool m_receiving = false;
};

#endif // AUDIOCONTROLLER_H
