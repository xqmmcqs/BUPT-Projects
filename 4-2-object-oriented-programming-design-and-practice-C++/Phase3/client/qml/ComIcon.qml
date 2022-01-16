import QtQuick 2.0

Image {
    height: 30
    width: 30
    source: {
        if (modelData.type === 0) return "/Images/clothes"
        else if (modelData.type === 1) return "/Images/book"
        else if (modelData.type === 2) return "/Images/elec"
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            client_model.itemGetDetail(modelData.id)
            stackView.push("ComView.qml")
        }
    }
}
