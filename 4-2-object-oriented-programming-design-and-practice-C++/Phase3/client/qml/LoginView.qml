import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

Item {
    id: root
    anchors.fill: parent

    BackButton {}

    Column {
        width: 400
        spacing: 20
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        RegularText {
            id: banner
            width: 400
            text: qsTr("登录页")
            font.pixelSize: 17
            horizontalAlignment: Text.AlignHCenter
        }

        CusTextField {
            id: usernameField
            placeholderText: "用户名"
        }

        CusTextField {
            id: passwdField
            placeholderText: "密码"
            echoMode: TextInput.Password
        }


        Row {
            width: 300
            height: 50
            spacing: 200
            anchors.horizontalCenter: parent.horizontalCenter

            BigButton {
                source: "/Images/login"
                mouseArea.onClicked: {
                    const msg = client_model.userLogin(usernameField.value, passwdField.value)
                    if(msg) {
                        error.text = "登录失败，" + msg
                        error.open()
                    }
                    else {
                        stackView.pop()
                    }
                }
            }

            BigButton {
                source: "/Images/register"
                mouseArea.onClicked: {
                    regDialog.open()
                }
            }
        }
    }

    TextDialog {
        id: regDialog
        title: "请选择用户类型"
        textField.visible: false
        ComboBox {
            id: typeBox
            anchors.centerIn: parent
            width: 150
            height: 30
            model: [ "商家", "消费者" ]
        }
        onAccepted : {
            const msg = client_model.userRegister(usernameField.value, passwdField.value, typeBox.currentIndex)
            if(msg) {
                error.text = "注册失败，" + msg
                error.open()
            }
            else {
                info.text = "注册成功"
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
