#include <QtQuick>
#include <auroraapp.h>

#include "audiocontroller.h"
#include "radiostate.h"
#include "signalingclient.h"

int main(int argc, char *argv[])
{
    QScopedPointer<QGuiApplication> application(Aurora::Application::application(argc, argv));
    application->setOrganizationName(QStringLiteral("ru.example"));
    application->setApplicationName(QStringLiteral("PocRadio"));
    application->setApplicationVersion(QStringLiteral("0.1.0"));

    qmlRegisterUncreatableType<RadioState>("ru.example.PocRadio", 1, 0, "RadioState",
                                           QStringLiteral("RadioState is provided by the radio context property"));

    SignalingClient signalingClient;
    AudioController audioController;
    RadioState radio(&signalingClient, &audioController);

    QObject::connect(application.data(), &QGuiApplication::applicationStateChanged,
                     &radio, [&radio](Qt::ApplicationState state) {
        radio.setApplicationActive(state == Qt::ApplicationActive);
    });
    QObject::connect(application.data(), &QCoreApplication::aboutToQuit,
                     &audioController, &AudioController::stopAll);

    QScopedPointer<QQuickView> view(Aurora::Application::createView());
    view->rootContext()->setContextProperty(QStringLiteral("radio"), &radio);
    view->rootContext()->setContextProperty(QStringLiteral("audioController"), &audioController);
    view->setSource(Aurora::Application::pathTo(QStringLiteral("qml/PocRadio.qml")));

    if (view->status() == QQuickView::Error) {
        qWarning() << "QML loading errors:" << view->errors();
        return 1;
    }

    radio.setApplicationActive(application->applicationState() == Qt::ApplicationActive);
    view->show();
    return application->exec();
}
