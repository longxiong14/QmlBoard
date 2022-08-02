import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import hUIControl 1.0
ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("Scroll")


    Flow{
        id:idflow
        width: parent.width
        height: 30
        Repeater{
            model: idControl.handleList()
            delegate: Button{
                text: idControl.handleList()[index]
                onClicked: {
                    idControl.setBoardHandle(idBoard.name, text)
                }
            }
        }
        Button{
            text: "test"
            onClicked: {
                idControl.test()
            }
        }
        Button{
            text: "visible"
            onClicked: {
                idBoard.visibleNode(false)
            }
        }

        Rectangle{
            width: 500
            height: 500
            border.color: "red"
            border.width: 2

            HBoard{
                id:idBoard
                anchors.fill: parent
                onHoverPoint: {
                    hoverd.text = "x:"+x + " y:"+y
                }
                name: "test_board"
            }
        }
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
