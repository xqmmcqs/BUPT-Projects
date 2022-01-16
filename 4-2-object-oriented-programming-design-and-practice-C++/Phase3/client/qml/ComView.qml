import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.1

Item {
    id: root
    anchors.fill: parent

    BackButton {}

    Row {
        id: profile
        width: 700
        height: 200
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: 50
        anchors.leftMargin: 50
        spacing: 10

        Image {
            width: 200
            height: 200
            source: client_model.singleCommodity.type === 0 ? "/Images/clothes" : client_model.singleCommodity.type === 1 ? "/Images/book" : "/Images/elec"
        }

        Column {
            height: 200
            spacing: 10

            Row {
                spacing: 20
                RegularText {
                    text: qsTr("名称")
                    height: 20
                    width: 30
                }

                RegularText {
                    text: client_model.singleCommodity.name
                    height: 20
                    width: 150
                }
            }

            Row {
                spacing: 20
                RegularText {
                    text: qsTr("类型")
                    height: 20
                    width: 30
                }

                RegularText {
                    text: client_model.singleCommodity.type === 0 ? "服饰" : client_model.singleCommodity.type === 1 ? "书籍" : "电子产品"
                    height: 20
                    width: 50
                }
            }

            Row {
                spacing: 20
                RegularText {
                    text: qsTr("价格")
                    height: 20
                    width: 30
                }

                RegularText {
                    text: (client_model.singleCommodity.price * client_model.singleCommodity.discount / 10000.0).toFixed(2)
                    height: 20
                    width: 50
                }

                Image {
                    source: "/Images/coupon"
                    visible: client_model.singleCommodity.discount < 100
                    height: 20
                    width: 20
                }

                RegularText {
                    text: (client_model.singleCommodity.price / 100.0).toFixed(2)
                    font.strikeout: true
                    visible: client_model.singleCommodity.discount < 100
                    height: 20
                    width: 50
                }
            }

            Row {
                spacing: 20
                RegularText {
                    text: qsTr("数量")
                    height: 20
                    width: 30
                }

                RegularText {
                    text: client_model.singleCommodity.amount
                    height: 20
                    width: 50
                }
            }

            BigButton {
                source: "/Images/addcart"
                mouseArea.onClicked: {
                    if(client_model.userActive) {
                        addCartDialog.textField.text = ""
                        addCartDialog.open()
                    }
                    else {
                        error.text = "未登录，请登录后使用购物车"
                        error.open()
                    }
                }
            }
        }
    }

    RegularText {
        width: 50
        text: "商品介绍"
        anchors.top: profile.bottom
        anchors.topMargin: 20
        anchors.left: introText.left
    }

    RegularText {
        id: introText
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        anchors.top: profile.bottom
        anchors.topMargin: 60
        anchors.left: profile.left;
        anchors.right: parent.right
        anchors.rightMargin: 50
        verticalAlignment: Text.AlignTop
        text: client_model.singleCommodity.intro
        wrapMode: Text.Wrap
    }

    TextDialog {
        id: addCartDialog
        title: "请填写加入购物车的数量"
        textField.placeholderText: qsTr("加入购物车的数量")
        onAccepted : {
            const msg = client_model.cartAdd(client_model.singleCommodity.id, textField.value)
            if(msg) {
                error.text = "添加到购物车失败，" + msg
                error.open()
            }
            else {
                info.text = "添加到购物车成功"
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
        icon: StandardIcon.Information
        title: "警告"
    }
}
