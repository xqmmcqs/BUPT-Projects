import QtQuick 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15

Item {
    id: root
    anchors.fill: parent

    onFocusChanged: {
        if(focus) {
            client_model.orderGetMine();
        }
    }

    BackButton {}

    BannerText {
        id: banner
        text: qsTr("订单列表")
    }

    Column {
        id: orderTable
        width: 190
        anchors.bottom: parent.bottom
        anchors.top: banner.bottom
        anchors.bottomMargin: 20
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            height: 30
            spacing: 10
            RegularText {
                text: qsTr(" ")
                width: 30
            }
            RegularText {
                text: qsTr("订单ID")
                width: 60
            }
            RegularText {
                text: qsTr("金额")
                width: 80
                horizontalAlignment: Text.AlignRight
            }
        }

        ScrollView {
            id: orderScroll
            width: 190
            height: parent.height - 40
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                anchors.fill: orderScroll
                model:client_model.order
                spacing: 10
                delegate: Item {
                    height: 30
                    Row {
                        height: 30
                        spacing: 10
                        SmallButton {
                            source: "/Images/box"
                            mouseArea.onClicked: {
                                stackView.push("OrderView.qml")
                                client_model.orderGetDetail(modelData.id)
                            }
                        }

                        Rectangle {
                            id: idRec
                            width: 60
                            height: 30
                            color: "transparent"
                            radius: 10
                            RegularText {
                                text: modelData.id
                                width: 50
                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onEntered: {
                                        parent.color = "blue"
                                        idRec.border.color = "lightgrey"
                                    }
                                    onExited: {
                                        parent.color = "black"
                                        idRec.border.color = "transparent"
                                    }
                                    onClicked: {
                                        stackView.push("OrderView.qml")
                                        client_model.orderGetDetail(modelData.id)
                                    }
                                }
                            }
                        }
                        RegularText {
                            text: (modelData.total / 100.0).toFixed(2)
                            width: 80
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        height: orderTable.height + 10
        width: orderTable.width + 10
        anchors.centerIn: orderTable
        color: "transparent"
        border.color: "lightgrey"
        radius: 5
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
