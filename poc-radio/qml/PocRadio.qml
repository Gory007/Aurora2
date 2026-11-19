import QtQuick 2.0
import Sailfish.Silica 1.0
import Nemo.KeepAlive 1.2
import ru.example.PocRadio 1.0

ApplicationWindow {
    id: applicationWindow

    readonly property bool applicationActive: Qt.application.state === Qt.ApplicationActive

    objectName: "applicationWindow"
    allowedOrientations: Orientation.All
    initialPage: Qt.resolvedUrl("pages/MainPage.qml")
    cover: Qt.resolvedUrl("cover/DefaultCoverPage.qml")

    // Документированная замена устаревших/непубличных keep-locks.
    // В пассивном LISTENING блокировка не расходует батарею.
    KeepAlive {
        enabled: radio.keepAlive
    }

    DisplayBlanking {
        preventBlanking: applicationWindow.applicationActive
                         && radio.mode === RadioState.Transmitting
    }
}
