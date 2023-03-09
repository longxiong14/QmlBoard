import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import QtQuick.Dialogs 1.3
import hNodes 1.0
import hUIControl 1.0

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("language")

    property var handles: idControl.handleList()
    HUIControl{
        id:idControl
    }

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
            text: qsTr("Chinese")
            onClicked: {
                trans.currentLang = "简体中文"
                idBoard.home()
            }
        }

        Button{
            text: qsTr("English")
            onClicked: {
                trans.currentLang = "English"
                idBoard.home()
            }
        }
        Button{
            text: "test"
            onClicked:idNodes.test()
        }
    }
    HNodes{
        id:idNodes
    }
    Flow{
        id:idButtonsFlow
        width: parent.width - 100
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
}
