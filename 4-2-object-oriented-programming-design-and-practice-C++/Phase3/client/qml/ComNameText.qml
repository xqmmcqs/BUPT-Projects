import QtQuick 2.0

Rectangle {
    id: root
    width: 210
    height: 30
    color: "transparent"
    radius: 10
    RegularText {
        text: modelData.name
        width: 200
        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: {
                parent.color = "blue"
                root.border.color = "lightgrey"
            }
            onExited: {
                parent.color = "black"
                root.border.color = "transparent"
            }
            onClicked: {
                client_model.itemGetDetail(modelData.id)
                stackView.push("ComView.qml")
            }
        }
    }
}
