import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.1

Item {
    id: root
    anchors.fill: parent

    function getStatusText(status) {
        if(status === 0) return "未支付"
        else if(status === 1) return "已取消"
        else if(status === 2) return "已支付"
        else if(status === 3) return "正在退款"
        else if(status === 4) return "已退款"
    }

    BackButton {}

    Image {
        id: orderImage
        width: 200
        height: 200
        source: "/Images/box"
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.left: parent.left
        anchors.leftMargin: 50
    }

    Row {
        id: mainOrder
        height: 30
        width: 490
        anchors.left: orderImage.right
        anchors.leftMargin: 10
        anchors.top: orderImage.top
        spacing: 10
        RegularText {
            text: qsTr("主订单ID  ") + client_model.mainOrder.id
            width: 130
        }
        RegularText {
            text: qsTr("状态  ") + getStatusText(client_model.mainOrder.status)
            width: 105
        }
        RegularText {
            text: qsTr("金额  ") + (client_model.mainOrder.total / 100.0).toFixed(2)
            width: 105
        }
        SmallButton {
            source: "/Images/pay"
            visible: client_model.mainOrder.status === 0
            mouseArea.onClicked: {
                payDialog.payId = client_model.mainOrder.id
                payDialog.open()
            }
        }
        SmallButton {
            source: "/Images/trash"
            visible: client_model.mainOrder.status === 0
            mouseArea.onClicked: {
                cancelDialog.cancelId = client_model.mainOrder.id
                cancelDialog.open()
            }
        }
    }

    Column {
        id: orderTable
        anchors.left: orderImage.right
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.top: mainOrder.bottom
        anchors.topMargin: 10
        anchors.bottom: orderImage.bottom

        Row {
            height: 30
            spacing: 10
            RegularText {
                text: qsTr("子订单ID")
                width: 60
            }
            RegularText {
                text: qsTr("状态")
                width: 60
            }
            RegularText {
                text: qsTr("买家")
                width: 105
            }
            RegularText {
                text: qsTr("卖家")
                width: 105
            }
            RegularText {
                text: qsTr("金额")
                width: 80
                horizontalAlignment: Text.AlignRight
            }
        }

        ScrollView {
            id: orderScroll
            width: 490
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
                        RegularText {
                            text: modelData.id
                            width: 60
                        }
                        RegularText {
                            text: getStatusText(modelData.status)
                            width: 60
                        }
                        RegularText {
                            text: modelData.buyer
                            width: 105
                        }
                        RegularText {
                            text: modelData.seller
                            width: 105
                        }
                        RegularText {
                            text: (modelData.total / 100.0).toFixed(2)
                            width: 80
                            horizontalAlignment: Text.AlignRight
                        }
                        SmallButton {
                            source: "/Images/refund"
                            visible: modelData.status === 2 || modelData.status === 3
                            mouseArea.onClicked: {
                                refundDialog.refundId = modelData.id
                                refundDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }

    Column {
        id: commodityTable
        width: 410
        anchors.bottom: parent.bottom
        anchors.top: orderImage.bottom
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
                text: qsTr("所属子订单ID")
                width: 60
            }
            RegularText {
                text: qsTr("数量")
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
                            text: modelData.order
                            width: 60
                        }
                        RegularText {
                            text: modelData.amount
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

    Rectangle {
        height: commodityTable.height + 10
        width: commodityTable.width + 10
        anchors.centerIn: commodityTable
        color: "transparent"
        border.color: "lightgrey"
        radius: 5
    }

    TextDialog {
        id: payDialog
        property int payId: -1
        title: "确认支付订单" + payId
        Text {
            text: "确认支付订单" + payId
        }
        textField.visible: false
        onAccepted : {
            const msg = client_model.orderPay(payId)
            if(msg) {
                error.text = "支付失败，" + msg
                error.open()
            }
            else {
                client_model.orderGetDetail(client_model.mainOrder.id)
            }
        }
    }

    TextDialog {
        id: cancelDialog
        property int cancelId: -1
        title: "确认取消订单" + cancelId
        Text {
            text: "确认取消订单" + cancelId
        }
        textField.visible: false
        onAccepted : {
            const msg = client_model.orderCancel(cancelId)
            if(msg) {
                error.text = "取消失败，" + msg
                error.open()
            }
            else {
                client_model.orderGetDetail(client_model.mainOrder.id)
            }
        }
    }

    TextDialog {
        id: refundDialog
        property int refundId: -1
        title: "确认退款订单" + refundId
        Text {
            text: "确认退款订单" + refundId
        }
        textField.visible: false
        onAccepted : {
            const msg = client_model.orderRefund(refundId)
            if(msg) {
                error.text = "退款失败，" + msg
                error.open()
            }
            else {
                client_model.orderGetDetail(client_model.mainOrder.id)
            }
        }
    }

    MessageDialog {
        id: error
        icon: StandardIcon.Warning
        title: "错误"
    }
}
