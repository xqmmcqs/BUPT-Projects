import QtQuick 2.12
import QtQuick.Controls 2.15

ApplicationWindow {
    width: 800
    height: 600
    visible: true
    title: qsTr("电商交易管理系统")
    minimumWidth: 800
    minimumHeight: 600
    color: "aliceblue"
    property alias stackView: stackView

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: mainView
    }

    Connections {
        target: client_model
    }

    MainView {
        id: mainView
    }
}
