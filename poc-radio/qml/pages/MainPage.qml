import QtQuick 2.0
import Sailfish.Silica 1.0
import ru.example.PocRadio 1.0

Page {
    id: page

    allowedOrientations: Orientation.All

    ListModel {
        id: logModel
    }

    Connections {
        target: radio
        onLogEntry: {
            logModel.insert(0, { "message": text })
            if (logModel.count > 6)
                logModel.remove(logModel.count - 1)
        }
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: contentColumn.height + Theme.paddingLarge

        Column {
            id: contentColumn
            width: parent.width
            spacing: Theme.paddingMedium

            PageHeader {
                title: qsTr("PoC Рация")
                description: qsTr("Этап 1: каркас и состояния")
            }

            Label {
                anchors.horizontalCenter: parent.horizontalCenter
                text: radio.modeName
                color: radio.mode === RadioState.Transmitting
                       ? Theme.errorColor : Theme.highlightColor
                font.pixelSize: Theme.fontSizeLarge
                font.bold: true
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                text: radio.statusText
                color: Theme.secondaryHighlightColor
            }

            SectionHeader {
                text: qsTr("Подключение")
            }

            TextSwitch {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                enabled: radio.mode === RadioState.Idle && !radio.joinPending
                checked: radio.simulationMode
                text: qsTr("Локальная симуляция")
                description: qsTr("Состояния без сети и сервера")
                onClicked: radio.setSimulationMode(checked)
            }

            TextField {
                id: serverField
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                visible: !radio.simulationMode
                enabled: radio.mode === RadioState.Idle && !radio.joinPending
                label: qsTr("Адрес сервера")
                placeholderText: qsTr("Например, 10.0.2.2")
                text: "10.0.2.2"
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            TextField {
                id: portField
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                visible: !radio.simulationMode
                enabled: radio.mode === RadioState.Idle && !radio.joinPending
                label: qsTr("Порт")
                text: "5000"
                inputMethodHints: Qt.ImhDigitsOnly
                validator: IntValidator { bottom: 1; top: 65535 }
            }

            TextField {
                id: channelField
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                enabled: radio.mode === RadioState.Idle && !radio.joinPending
                label: qsTr("Канал")
                placeholderText: qsTr("Имя канала")
                text: "test-channel"
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            TextField {
                id: nameField
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                enabled: radio.mode === RadioState.Idle && !radio.joinPending
                label: qsTr("Ваше имя")
                placeholderText: qsTr("Имя участника")
                text: "Ivan"
                inputMethodHints: Qt.ImhNoPredictiveText
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                enabled: true
                text: radio.joinPending ? qsTr("Отмена")
                                        : (radio.mode === RadioState.Idle
                                           ? qsTr("Войти в канал") : qsTr("Выйти из канала"))
                onClicked: {
                    if (radio.joinPending) {
                        radio.leaveChannel()
                    } else if (radio.mode === RadioState.Idle) {
                        radio.connectToChannel(serverField.text, parseInt(portField.text),
                                               channelField.text, nameField.text)
                    } else {
                        radio.leaveChannel()
                    }
                }
            }

            SectionHeader {
                text: qsTr("Эфир")
            }

            Rectangle {
                id: pttButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: Math.min(parent.width - 2 * Theme.horizontalPageMargin,
                                Theme.itemSizeHuge * 3)
                height: width
                radius: width / 2
                opacity: pttArea.enabled ? 1.0 : 0.35
                color: radio.mode === RadioState.Transmitting
                       ? Theme.errorColor
                       : (pttArea.pressed ? Theme.highlightColor : Theme.secondaryHighlightColor)

                Label {
                    anchors.centerIn: parent
                    width: parent.width - 2 * Theme.paddingLarge
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.WordWrap
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeLarge
                    font.bold: true
                    text: radio.mode === RadioState.Transmitting
                          ? qsTr("ГОВОРИТЕ")
                          : (radio.pttPending ? qsTr("ЗАПРОС…")
                                              : qsTr("УДЕРЖИВАЙТЕ\nPTT"))
                }

                MouseArea {
                    id: pttArea
                    anchors.fill: parent
                    preventStealing: true
                    enabled: radio.applicationActive
                             && (pttArea.pressed || radio.canTalk || radio.pttPending
                                 || radio.mode === RadioState.Transmitting)
                    onPressed: radio.requestTalk()
                    onReleased: radio.releaseTalk()
                    onCanceled: radio.releaseTalk()
                }

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            Label {
                x: Theme.horizontalPageMargin
                width: parent.width - 2 * Theme.horizontalPageMargin
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.secondaryColor
                text: qsTr("При сворачивании передача автоматически завершится. Звук на этом этапе не передаётся.")
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                visible: radio.simulationMode && radio.mode !== RadioState.Idle
                enabled: radio.mode === RadioState.Listening && !radio.pttPending
                text: radio.remoteSpeaker ? qsTr("Завершить входящую передачу")
                                          : qsTr("Имитировать входящую передачу")
                onClicked: radio.simulateIncomingTransmission()
            }

            SectionHeader {
                text: qsTr("Журнал")
            }

            Repeater {
                model: logModel
                delegate: Label {
                    x: Theme.horizontalPageMargin
                    width: contentColumn.width - 2 * Theme.horizontalPageMargin
                    wrapMode: Text.WordWrap
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.secondaryColor
                    text: model.message
                }
            }
        }
    }
}
