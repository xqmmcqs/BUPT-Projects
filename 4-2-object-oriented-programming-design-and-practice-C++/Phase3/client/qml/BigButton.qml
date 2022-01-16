import QtQuick 2.0

Image {
    property alias mouseArea: mouseArea
    width: 50
    height: 50
    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}
