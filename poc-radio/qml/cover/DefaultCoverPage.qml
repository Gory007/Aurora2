import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Column {
        anchors.centerIn: parent
        width: parent.width - 2 * Theme.paddingMedium
        spacing: Theme.paddingSmall

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("PoC Рация")
            color: Theme.highlightColor
            font.bold: true
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            text: radio.modeName
            font.pixelSize: Theme.fontSizeSmall
        }

        Label {
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            visible: radio.remoteSpeaker
            text: radio.remoteSpeaker ? qsTr("Говорит %1").arg(radio.speakerName) : ""
            font.pixelSize: Theme.fontSizeExtraSmall
        }
    }
}
