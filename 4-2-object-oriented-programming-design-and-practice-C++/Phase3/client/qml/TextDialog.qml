import QtQuick 2.12
import QtQuick.Controls 2.15
import QtQuick.Dialogs 1.2

Dialog {
    property alias textField: dialogTextField
    standardButtons: Dialog.Ok | Dialog.Cancel
    CusTextField {
        id: dialogTextField
        width: 150
        anchors.horizontalCenter: parent.horizontalCenter
    }
    visible: false
}
