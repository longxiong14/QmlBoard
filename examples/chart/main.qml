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

//        HHistogramChart{
//            width: parent.width - 20
//            height: 100
//            array: [17,0,1,3,6,6,9,12,11,16,16,10,8,9,8,6,7,3,4,0,4,1,3,2,2,5,1,2,3,1,2,0,2,0,0,1,0,0,3,1,0,0,1,1,0,0,2,0,4,1,0,4,1,3,3,2,3,0,3,2,8,6,7,3,5,17,17,24,22,24,41,61,56,73,69,95,109,114,129,141,156,152,159,173,184,177,200,184,194,169,212,194,199,162,185,153,182,145,128,145,107,93,105,82,82,61,50,54,45,46,35,33,31,23,30,22,18,17,12,11,15,6,10,10,10,7,2,1,7,2,3,3,2,2,2,4,0,1,2,1,1,1,2,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
//            17,0,1,3,6,6,9,12,11,16,16,10,8,9,8,6,7,3,4,0,4,1,3,2,2,5,1,2,3,1,2,0,2,0,0,1,0,0,3,1,0,0,1,1,0,0,2,0,4,1,0,4,1,3,3,2,3,0,3,2,8,6,7,3,5,17,17,24,22,24,41,61,56,73,69,95,109,114,129,141,156,152,159,173,184,177,200,184,194,169,212,194,199,162,185,153,182,145,128,145,107,93,105,82,82,61,50,54,45,46,35,33,31,23,30,22,18,17,12,11,15,6,10,10,10,7,2,1,7,2,3,3,2,2,2,4,0,1,2,1,1,1,2,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]
//            color: "lightblue"
//        }


        Repeater{
            id:idRepeater
            delegate: HHistogramChart{
               width: parent.width - 20
               height: 100
               array: gHandle.charts[index]
               color: colors[index]
//               lineColor:"pink"
//               lineWidth:5
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
