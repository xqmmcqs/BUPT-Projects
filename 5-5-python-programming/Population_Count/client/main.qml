import QtQuick 2.15
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 1440
    height: 640
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width
    title: "全球人口分布查询"

    Connections {
        target: client_model
    }

    Image {
        id: map
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.topMargin: 10
        anchors.leftMargin: 10
        width: parent.width - 200
        source: "./map.jpg"
        fillMode: Image.PreserveAspectFit
        MouseArea {
            id: mouse_area
            anchors.fill: parent
            onClicked: client_model.add_point(mouseX / width * 1296000 - 648000, -(mouseY / height * 648000 - 324000))
        }
    }

    Column {
        anchors.top: parent.top
        anchors.left: map.right
        anchors.topMargin: 10
        anchors.leftMargin: 10
        height: map.height - 30
        width: 170

        Row {
            id: title
            height: 30
            Text {
                text: "经度"
                width: 70
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                text: "纬度"
                width: 70
                verticalAlignment: Text.AlignVCenter
            }
            Text {
                text: ""
                width: 30
                verticalAlignment: Text.AlignVCenter
            }
        }

        ListView {
            id: list_view
            anchors.left: parent.left
            height: parent.height - 30
            width: parent.width
            model: client_model.coordinate_list
            clip: true

            delegate: Row {
                property int indexOfThisDelegate: index
                height: 30
                Text {
                    text: modelData.x_deg + "°" + modelData.x_min + "′" + modelData.x_sec + "″"
                    width: 70
                    verticalAlignment: Text.AlignVCenter
                }
                Text {
                    text: modelData.y_deg + "°" + modelData.y_min + "′" + modelData.y_sec + "″"
                    width: 70
                    verticalAlignment: Text.AlignVCenter
                }
                Button {
                    width: 30
                    height: 20
                    text: "删"
                    font.pixelSize: 8
                    onClicked: client_model.remove_point(index)
                }
            }
            onCountChanged: {
                list_view.positionViewAtEnd()
            }
            ScrollBar.vertical: ScrollBar {
                active: true
            }
        }

        Button {
            anchors.right: list_view.right
            width: 80
            height: 30
            text: "提交"
            font.pixelSize: 15
            onClicked: client_model.submit()
        }
    }

    MessageDialog {
        id: error
        icon: StandardIcon.Warning
    }

    function errorOccurred(str) {
        error.text = str
        error.open()
    }
}