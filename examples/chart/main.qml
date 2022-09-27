import QtQuick 2.12
import QtQuick.Controls 2.5
import hBoard 1.0
import QtQuick.Dialogs 1.3
import hHistogramChart 1.0

ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("chart handle")
    property var colors: ["blue","green","red"]

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

    Flow{
        id:idRightFlow
        width: 300
        height: parent.height
        anchors.right: parent.right
        Rectangle{
            width: parent.width
            height: 30
            Text {
                anchors.centerIn: parent
                text: qsTr("直方图")
            }
        }

        Repeater{
            id:idRepeater
            delegate: HHistogramChart{
               width: parent.width - 20
               height: 100
               array: gHandle.charts[index]
               color: colors[index]
            }
        }
    }

    HBoard{
        id:idBoard
        anchors{
            left: parent.left
            top: idButtonsFlow.bottom
            right: idRightFlow.left
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


    FileDialog{
        id:idFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
            gHandle.openPicture(destPath, idBoard.name)
        }
    }

    Connections{
        target: gHandle
        onChartsChanged:{
            idRepeater.model = 0
            idRepeater.model = gHandle.charts
        }
    }

}
