import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import QtQuick.Dialogs 1.3


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

}
