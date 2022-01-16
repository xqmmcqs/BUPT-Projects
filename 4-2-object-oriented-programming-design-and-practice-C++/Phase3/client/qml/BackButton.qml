import QtQuick 2.0

BigButton {
    source: "/Images/back"
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.topMargin: 10
    anchors.leftMargin: 10
    mouseArea.onClicked: {
        stackView.pop();
    }
}
