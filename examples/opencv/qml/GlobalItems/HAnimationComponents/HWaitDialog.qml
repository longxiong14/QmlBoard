import QtQuick 2.0
import "../HBaseItem"
HWindow {
    property int waitTime: 5000
    property int count: 0
    signal cancle()
    id:idWindow

//    flags: Qt.Widget
//    modality: Qt.WindowModal
    flags: Qt.Window | Qt.FramelessWindowHint
    modality: Qt.WindowModal

    color: gModel.backGround

    HWaitComponent{
        id:idComponent
        width: parent.width
        height: 100

//        width: 100
//        height: 100
        anchors.centerIn: parent
//        text: title + " use time : " + String(count) + "s" + trans.transString
        text: title + qsTr("\nuse time : %1 s").arg(count) + trans.transString
    }

    HButton{
        id:idButtons
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        text: qsTr("cancle") + trans.transString
        width: 80
        onClicked: {
            cancle()
        }
    }

    Timer{
        id:idTimer
        repeat: true
        interval: 1000
        onRunningChanged: if(!running) count = 0
        onTriggered: count++
    }

    onVisibleChanged: {
        if(visible) {
            idTimer.restart()
        } else {
            count = 0
            idTimer.stop()
        }
    }
}
