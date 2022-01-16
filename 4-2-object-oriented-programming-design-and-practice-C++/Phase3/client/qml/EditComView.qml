import QtQuick 2.0
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

Item {
    id: root
    anchors.fill: parent

    BackButton {}

    Column {
        width: 400
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 50
        spacing: 20

        RegularText {
            width: 400
            text: qsTr("修改商品")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
        }

        CusTextField {
            id: nameField
            placeholderText: "名称  " + client_model.singleCommodity.name
        }

        CusTextField {
            id: introField
            placeholderText: "介绍  " + client_model.singleCommodity.intro
        }

        CusTextField {
            id: priceField
            placeholderText: "价格  " + (client_model.singleCommodity.price / 100.0).toFixed(2)
        }

        CusTextField {
            id: amountField
            placeholderText: "数量  " + client_model.singleCommodity.amount
        }

        Row {
            width: 300
            height: 50
            spacing: 200
            anchors.horizontalCenter: parent.horizontalCenter

            BigButton {
                source: "/Images/check"
                mouseArea.onClicked: {
                    const msg = client_model.itemSet(client_model.singleCommodity.id, nameField.value, introField.value, priceField.value, amountField.value)
                    if(msg) {
                        error.text = "修改失败，" + msg
                        error.open()
                    }
                    else {
                        stackView.pop()
                    }
                }
            }

            BigButton {
                source: "/Images/delete"
                mouseArea.onClicked: {
                    stackView.pop()
                }
            }
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
