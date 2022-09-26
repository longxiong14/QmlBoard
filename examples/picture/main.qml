import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import hUIControl 1.0
import QtQuick.Dialogs 1.3


ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("open picture")

    Flow{
        id:idButtonsFlow
        width: parent.width
        Button{
            text: "open"
            onClicked: {
                idFile.open()
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

    FileDialog{
        id:idFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
            idControl.openBoardPicture(idBoard.name, destPath)
        }
    }

    FileDialog{
        id:idCppFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
        }
    }
}
