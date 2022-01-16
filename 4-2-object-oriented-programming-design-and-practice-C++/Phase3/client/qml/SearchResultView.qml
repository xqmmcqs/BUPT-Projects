import QtQuick 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15

Item {
    id: root
    anchors.fill: parent

    BackButton {}

    BannerText {
        id: banner
        text: qsTr("搜索结果")
    }

    Column {
        id: commodityTable
        width: 420
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
                text: qsTr("名称")
                width: 210
            }
            RegularText {
                text: qsTr("数量")
                width: 60
                horizontalAlignment: Text.AlignRight
            }
            RegularText {
                text: qsTr("价格")
                width: 80
                horizontalAlignment: Text.AlignRight
            }
        }

        ScrollView {
            id: commodityScroll
            width: 420
            height: parent.height - 40
            clip: true
            ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

            ListView {
                anchors.fill: commodityScroll
                model:client_model.commodity
                spacing: 10
                delegate: Item {
                    height: 30
                    Row {
                        height: 30
                        spacing: 10
                        ComIcon {}
                        ComNameText {}
                        RegularText {
                            text: modelData.amount
                            width: 60
                            horizontalAlignment: Text.AlignRight
                        }
                        RegularText {
                            text: (modelData.price * modelData.discount / 10000.0).toFixed(2)
                            width: 80
                            horizontalAlignment: Text.AlignRight
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        height: commodityTable.height + 10
        width: commodityTable.width + 10
        anchors.centerIn: commodityTable
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
