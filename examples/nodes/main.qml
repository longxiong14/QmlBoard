import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import QtQuick.Dialogs 1.3
import hNodes 1.0

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("nodes")




    HBoard{
        id:idBoard
        anchors.margins: 5
        anchors.fill: parent
        onHoverPoint: {
            hoverd.text = "x:"+x + " y:"+y
        }
        name: "test_board"
    }

    Text {
        id: hoverd
        anchors.bottom: parent.bottom
        text: qsTr("text")
    }

    Flow{
        width: 100
        anchors.right: parent.right

        Button{
            text: "push"
            onClicked: idNodes.push()
        }

        Button{
            text: "remove"
            onClicked: idNodes.clear()
        }

        Button{
            text: "save"
            onClicked: idNodes.save()
        }

        Button{
            text: "load"
            onClicked:idNodes.load()
        }
    }
    HNodes{
        id:idNodes
    }
}
