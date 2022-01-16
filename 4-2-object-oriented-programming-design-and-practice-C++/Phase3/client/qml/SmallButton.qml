import QtQuick 2.0

Image {
    property alias mouseArea: mouseArea
    width: 30
    height: 30
    MouseArea {
        id: mouseArea
        anchors.fill: parent
    }
}
