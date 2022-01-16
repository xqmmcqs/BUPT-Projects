import QtQuick 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15

Item {
    id: root
    anchors.fill: parent

    onFocusChanged: {
        if(focus) {
            client_model.itemGetAll();
        }
    }

    BannerText {
        id: banner
        text: qsTr("主页")
    }

    Row {
        id: main
        width: 500
        height: 50
        anchors.top: banner.bottom
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 100

        BigButton {
            source: "/Images/search"
            mouseArea.onClicked: {
                stackView.push("SearchView.qml")
            }
        }

        BigButton {
            source: "/Images/orderlist"
            mouseArea.onClicked: {
                if(client_model.userActive) {
                    const msg = client_model.orderGetMine()
                    if(msg) {
                        error.text = "查看订单失败，" + msg
                        error.open()
                    }
                    else {
                        stackView.push("OrderListView.qml")
                    }
                }
                else {
                    stackView.push("LoginView.qml")
                }
            }
        }

        BigButton {
            source: "/Images/cart"
            mouseArea.onClicked: {
                if(client_model.userActive) {
                    const msg = client_model.cartGet()
                    if(msg) {
                        error.text = "查看购物车失败，" + msg
                        error.open()
                    }
                    else {
                        stackView.push("CartView.qml")
                    }
                }
                else {
                    stackView.push("LoginView.qml")
                }
            }
        }

        BigButton {
            source: "/Images/profile"
            mouseArea.onClicked: {
                if(client_model.userActive) {
                    stackView.push("UserView.qml")
                }
                else {
                    stackView.push("LoginView.qml")
                }
            }
        }
    }

    Column {
        id: commodityTable
        width: 410
        anchors.bottom: parent.bottom
        anchors.top: main.bottom
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
            width: 410
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

    BigButton {
        source: "/Images/offer"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.leftMargin: 10
        mouseArea.onClicked: {
            stackView.push("DiscountView.qml");
        }
    }

    MessageDialog {
        id: error
        icon: StandardIcon.Warning
        title: "错误"
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
