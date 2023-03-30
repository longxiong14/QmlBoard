import QtQuick 2.0
import "../HBaseItem"
Item {
    property string text: ""
    id:idItem
    Image {
        id:idImage
        anchors{
            fill:parent
        }
        source: "file:./configs/HIcons/HTools/wait.png"
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent
        rotation: 0
        RotationAnimation on rotation {
            running: idItem.visible
            from:0
            to: 360
            duration: 2000
            loops: Animation.Infinite
        }
    }
    HText {
        text: idItem.text
        anchors.centerIn: parent
        width: parent.width
        wrapMode: Text.WordWrap
    }
}
