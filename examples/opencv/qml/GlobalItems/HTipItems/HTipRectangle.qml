import QtQuick 2.0
import QtQuick.Controls 2.5
import "../HBaseItem"
Rectangle {
    property var tip: ""
    property bool error: true
    property string txtName: ""

    id: idMainRect
    MouseArea{
        hoverEnabled: true
        anchors.fill: parent
        onExited: idToolTip.visible = false
        onEntered: idToolTip.visible = true
    }

    ToolTip{
        id:idToolTip
        text: tip
    }

    HText{
        anchors.top: idMainRect.bottom
        anchors.topMargin: 5
        width: parent.width
        height: parent.height
        font.pixelSize: 14
        text: txtName
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Rectangle{
        id:circle
        width: radius*2
        height: radius*2
        radius: parent.width
        visible: error
        border.width: 4
        border.color: parent.color
        color: "transparent"
        anchors.centerIn: parent
        SequentialAnimation {
            id: animation
//            running: error
            loops: Animation.Infinite
            ParallelAnimation {
                NumberAnimation {
                    target: circle
                    property: "radius"
                    from: idMainRect.width / 2
                    to: idMainRect.width
                    duration: 500
                }
                NumberAnimation {
                    target: circle
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 500
                }
            }
        }
    }
    onErrorChanged: {
        changedStatus()
    }
    Component.onCompleted: changedStatus()
    function changedStatus(){
        if(error){
            animation.restart()
        }else{
            animation.stop()
        }
    }
}
