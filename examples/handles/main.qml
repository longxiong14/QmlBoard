﻿import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import hUIControl 1.0

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("handles")

    property var handles: idControl.handleList()
    Flow{
        id:idButtonsFlow
        width: parent.width
        spacing: 5
        Repeater{
            model: handles
            delegate: Button{
                text: handles[index]
                onClicked: {
                    idControl.setBoardHandle(idBoard.name,text)
                }
            }
        }
    }

    HBoard{
        id:idBoard
        anchors{
            left: parent.left
            top: idButtonsFlow.bottom
            right:parent.right
            bottom: parent.bottom
        }
        anchors.margins: 5
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

    HUIControl{
        id:idControl
    }
}