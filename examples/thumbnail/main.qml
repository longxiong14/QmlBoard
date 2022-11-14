import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import hUIControl 1.0
import QtQuick.Dialogs 1.3
import hThumbnail 1.0


ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("thnmbnail picture")

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
        onHoverPoint: {
            hoverd.text = "x:"+x + " y:"+y
        }
        name: "max_board"
    }

    HThumbnail{
        width: idBoard.width / 10
        height: idBoard.height / 10
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        rule: false
        name: "thunail_board"
    }

    Text {
        id: hoverd
        anchors.bottom: parent.bottom
        text: qsTr("text")
    }

    FileDialog{
        id:idFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)", "all (*.*)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
            gCtrl.open(destPath)
        }
    }
}
