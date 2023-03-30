import QtQuick 2.0
import QtQuick.Controls 2.0
import QtGraphicalEffects 1.0
import "../../GlobalItems/HBaseItem"

 HButton{
    id: expandBtn
    property bool isOpened: true
    property var fontSize: 20
    contentItem: HText {
        x: 0
        text: expandBtn.text
        color: /*expandBtn.textColor*/ "white"
        //horizontalAlignment: Text.AlignLeft
        verticalAlignment: Text.AlignVCenter
        font.bold: true
        font.pixelSize: fontSize
        anchors.fill: expandBtn

        anchors.leftMargin: 60
    }

    background: Rectangle {
        width: expandBtn.width
        height: expandBtn.height
        //color: expandBtn.pressed ? Qt.darker("lightblue", 1.4) : (expandBtn.hovered ? Qt.darker("lightblue", 1.2) : "#f5f5f6")
        color: Qt.darker(gModel.backGround,1.1)
        radius: 2
        Image {
            id: idExpandIcon
            source: "file:./configs/HIcons/HTools/expand.png"
            width: 24
            height: 24
            anchors {
                right: parent.right
                rightMargin: 14
                verticalCenter: parent.verticalCenter

            }
            rotation: expandBtn.isOpened ? 180 : 0
            Behavior on rotation {
                NumberAnimation {
                    duration: 200
                }
            }
            ColorOverlay{
                anchors.fill: idExpandIcon
                source: idExpandIcon
                cached: true
                color: "black"
            }
        }
    }
}
