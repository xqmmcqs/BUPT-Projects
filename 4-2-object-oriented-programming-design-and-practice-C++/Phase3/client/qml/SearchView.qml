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
            text: qsTr("搜索商品")
            font.pixelSize: 20
            horizontalAlignment: Text.AlignHCenter
        }

        CusTextField {
            id: nameField
            placeholderText: "名称"
        }

        ComboBox {
            id: typeBox
            width: 400
            height: 30
            model: [ " ", "服饰", "书籍", "电子产品" ]
        }

        CusTextField {
            id: minPriceField
            placeholderText: "最低价格"
        }

        CusTextField {
            id: maxPriceField
            placeholderText: "最高价格"
        }

        CheckBox {
            id: checkBox
            height: 30
            width: 400
            text: qsTr("忽略无货")
        }

        BigButton {
            anchors.horizontalCenter: parent.horizontalCenter
            source: "/Images/search"
            mouseArea.onClicked: {
                const msg = client_model.itemGetFilter(nameField.value, typeBox.currentIndex, minPriceField.value, maxPriceField.value, checkBox.checkState)
                if(msg) {
                    error.text = "搜索失败，" + msg
                    error.open()
                }
                else {
                    stackView.push("SearchResultView.qml")
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
