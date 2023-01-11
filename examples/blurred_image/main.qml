import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import QtQuick.Dialogs 1.3

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("blurred image")
    property var colors: ["blue","green","red"]

    HBoard{
        id:idBoard
        anchors.fill: parent
        anchors.margins: 5
        onHoverPoint: {
            hoverd.text = "x:"+x + " y:"+y
        }
        name: "test_board"
        onSigMouseRelease:{

        }
    }

    Text {
        id: hoverd
        anchors.bottom: parent.bottom
        text: qsTr("text")
    }


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

    FileDialog{
        id:idFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
            gCtrl.open(destPath, idBoard.name)
        }
    }
}
