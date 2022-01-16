import QtQuick 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15

Item {
    anchors.fill: parent

    onFocusChanged: {
        if(focus) {
            client_model.cartGet()
        }
    }

    BackButton {}

    BannerText {
        id: banner
        text: qsTr("购物车")
    }

    Column {
        id: commodityTable
        width: 540
        anchors.bottom: parent.bottom
        anchors.top: banner.bottom
        anchors.bottomMargin: 110
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            height: 30
            spacing: 10
            RegularText {
                text: qsTr(" ")
                width: 70
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
            width: 530
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

                        CheckBox {
                            id: checkBox
                            height: 30
                            width: 30
                            nextCheckState: function() {
                                if (checkState === Qt.Checked) {
                                    modelData.order = false
                                    orderText.calTotal()
                                    return Qt.Unchecked
                                }
                                else {
                                    modelData.order = true
                                    orderText.calTotal()
                                    return Qt.Checked
                                }
                            }
                        }
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

                        SmallButton {
                            source: "/Images/edit"
                            mouseArea.onClicked: {
                                editDialog.editId = modelData.id
                                editDialog.textField.value = ""
                                editDialog.open()
                            }
                        }

                        SmallButton {
                            source: "/Images/removecart"
                            mouseArea.onClicked: {
                                delDialog.delId = modelData.id
                                delDialog.open()
                            }
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
        id: orderButton
        source: "/Images/order"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.right: parent.right
        anchors.rightMargin: 50
        mouseArea.onClicked: {
            orderDialog.open()
        }
    }

    RegularText {
        id: orderText
        width: 100
        anchors.bottom: orderButton.top
        anchors.right: orderButton.right
        text: "总金额 " + (total / 10000.0).toFixed(2)
        horizontalAlignment: Text.AlignRight
        property int total: 0
        function calTotal() {
            total = 0
            for( var i = 0;i < client_model.commoditySize; i++ ) {
                total += client_model.commodity[i].amount * client_model.commodity[i].order * client_model.commodity[i].price * client_model.commodity[i].discount
            }
        }
    }

    TextDialog {
        id: editDialog
        property int editId: -1
        title: "修改商品" + editId + "数量"
        textField.placeholderText: qsTr("商品数量")
        onAccepted : {
            const msg = client_model.cartSet(editId, textField.value)
            if(msg) {
                error.text = "修改失败，" + msg
                error.open()
            }
            else {
                client_model.cartGet()
                orderText.calTotal()
            }
        }
    }

    TextDialog {
        id: delDialog
        property int delId: -1
        title: "确认移除商品" + delId
        Text {
            text: "确认移除商品" + delId
        }
        textField.visible: false
        onAccepted : {
            const msg = client_model.cartDelete(delId)
            if(msg) {
                error.text = "移除失败，" + msg
                error.open()
            }
            else {
                client_model.cartGet()
                orderText.calTotal()
            }
        }
    }

    TextDialog {
        id: orderDialog
        title: "确认下单"
        Text {
            text: "确认下单"
        }
        textField.visible: false
        onAccepted : {
            const msg = client_model.orderAdd()
            if(msg) {
                error.text = "下单失败，" + msg
                error.open()
            }
            else {
                client_model.cartGet()
                orderText.calTotal()
                info.text = "下单成功"
                info.open()
            }
        }
    }

    MessageDialog {
        id: error
        icon: StandardIcon.Warning
        title: "错误"
    }

    MessageDialog {
        id: info
        icon: StandardIcon.Infomation
        title: "警告"
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
