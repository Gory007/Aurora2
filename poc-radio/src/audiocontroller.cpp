#include "audiocontroller.h"

AudioController::AudioController(QObject *parent)
    : QObject(parent)
{
}

void AudioController::startTransmitting()
{
    if (m_transmitting)
        return;

    // Этап аудио: создать захват микрофона -> Opus -> UDP.
    m_transmitting = true;
    emit transmittingChanged(true);
    emit logEntry(QStringLiteral("Аудиотракт передачи активирован (заглушка этапа 1)"));
}

void AudioController::stopTransmitting()
{
    if (!m_transmitting)
        return;

    // Этап аудио: остановить захват и освободить ресурсы микрофона.
    m_transmitting = false;
    emit transmittingChanged(false);
    emit logEntry(QStringLiteral("Аудиотракт передачи остановлен"));
}

void AudioController::startReceiving()
{
    if (m_receiving)
        return;

    // Этап аудио: запустить UDP -> Opus -> воспроизведение.
    m_receiving = true;
    emit receivingChanged(true);
    emit logEntry(QStringLiteral("Аудиотракт приёма активирован (заглушка этапа 1)"));
}

void AudioController::stopReceiving()
{
    if (!m_receiving)
        return;

    m_receiving = false;
    emit receivingChanged(false);
    emit logEntry(QStringLiteral("Аудиотракт приёма остановлен"));
}

void AudioController::stopAll()
{
    stopTransmitting();
    stopReceiving();
}
