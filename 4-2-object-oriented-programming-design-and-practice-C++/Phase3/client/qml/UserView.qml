import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.1

Item {
    id: root
    anchors.fill: parent

    onFocusChanged: {
        if(focus) {
            client_model.userGetInfo();
            if(!client_model.user.type) client_model.itemGetMine();
        }
    }

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
            source: client_model.user.type ? "/Images/customer" : "/Images/merchant"
        }

        Column {
            height: 200
            spacing: 10
            Row {
                height: 30
                spacing: 10
                RegularText {
                    text: qsTr("用户名")
                    width: 60
                }
                RegularText {
                    text: client_model.user.username
                    width: 150
                }
            }

            Row {
                height: 30
                spacing: 10
                RegularText {
                    text: qsTr("用户类型")
                    width: 60
                }
                RegularText {
                    text: client_model.user.type ? qsTr("消费者") : qsTr("商家")
                    width: 50
                }
            }

            Row {
                height: 30
                spacing: 10
                RegularText {
                    text: qsTr("余额")
                    width: 60
                }

                RegularText {
                    text: (client_model.user.balance / 100.0).toFixed(2)
                    width: 100
                }

                SmallButton {
                    source: "/Images/plus"
                    mouseArea.onClicked: {
                        addDialog.textField.value = ""
                        addDialog.open()
                    }
                }

                SmallButton {
                    source: "/Images/minus"
                    mouseArea.onClicked: {
                        subDialog.textField.value = ""
                        subDialog.open()
                    }
                }
            }

            Row {
                height: 50
                spacing: 30

                BigButton {
                    source: "/Images/passwd"
                    mouseArea.onClicked: {
                        passDialog.textField.value = ""
                        passDialog.open()
                    }
                }

                BigButton {
                    source: "/Images/logout"
                    mouseArea.onClicked: {
                        client_model.userLogout()
                        stackView.pop()
                    }
                }
            }
        }
    }

    Column {
        id: commodityTable
        visible: client_model.user.type ? 0 : 1
        width: 500
        anchors.bottom: parent.bottom
        anchors.top: profile.bottom
        anchors.bottomMargin: 20
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        Row {
            height: 30
            spacing: 50
            RegularText {
                text: qsTr("我的商品")
                width: 50
            }
            SmallButton {
                source: "/Images/plus"
                mouseArea.onClicked: {
                    stackView.push("AddComView.qml")
                }
            }
        }

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
            width: 500
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

                        SmallButton {
                            source: "/Images/edit"
                            mouseArea.onClicked: {
                                client_model.itemGetDetail(modelData.id)
                                stackView.push("EditComView.qml")
                            }
                        }
                        SmallButton {
                            source: "/Images/trash"
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
        visible: commodityTable.visible
        anchors.centerIn: commodityTable
        color: "transparent"
        border.color: "lightgrey"
        radius: 5
    }

    TextDialog {
        id: addDialog
        title: "请填写充值金额"
        textField.placeholderText: qsTr("充值金额")
        onAccepted : {
            const msg = client_model.userAddMoney(addDialog.textField.value);
            if(msg) {
                error.text = "充值失败，" + msg
                error.open()
            }
            else {
                client_model.userGetInfo();
            }
        }
    }

    TextDialog {
        id: subDialog
        title: "请填写提现金额"
        textField.placeholderText: qsTr("提现金额")
        onAccepted : {
            const msg = client_model.userSubMoney(subDialog.textField.value);
            if(msg) {
                error.text = "提现失败，" + msg
                error.open()
            }
            else {
                client_model.userGetInfo();
            }
        }
    }

    TextDialog {
        id: passDialog
        title: "请填写新密码"
        textField.placeholderText: qsTr("新密码")
        textField.echoMode: TextInput.Password
        onAccepted : {
            const msg = client_model.userSetPasswd(passDialog.textField.value);
            if(msg) {
                error.text = "修改密码失败，" + msg
                error.open()
            }
            else {
                info.text = "修改密码成功"
                info.open()
            }
        }
    }

    TextDialog {
        id: delDialog
        property int delId: -1
        title: "确认删除商品" + delId
        textField.visible: false
        onAccepted : {
            const msg = client_model.itemDelete(delId);
            if(msg) {
                error.text = "删除失败，" + msg
                error.open()
            }
            else {
                client_model.itemGetMine();
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

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}D{i:22}
}
##^##*/
