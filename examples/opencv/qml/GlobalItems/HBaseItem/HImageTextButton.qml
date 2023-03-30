import QtQuick 2.0
import QtQuick.Controls 2.5
HRectangle{
    property var colorArray: ["transparent","#91C9F7","#00a2e8"]
    property string mSource: ""
    property string text: ""
    property int iconSize: 10
    signal sigClick()
    id:idMainRect
    color: colorArray[0]
    border.width: 1
    border.color: "#eee"

    onColorArrayChanged: color = colorArray[0]
    Column{
        anchors.centerIn: parent
        spacing: 5
        Image {
            x: (parent.width - iconSize) / 2
            id: idUpButton
            source: mSource
            width: iconSize
            height: iconSize
            smooth: true
            fillMode: Image.PreserveAspectFit
        }

        HText{
            id:idText
            text: idMainRect.text
            wrapMode: Text.WrapAnywhere
        }
    }

    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            idMainRect.color = colorArray[1]
        }
        onExited: {
            idMainRect.color = colorArray[0]
        }
        onPressed: {
            idMainRect.color = colorArray[2]
        }
        onReleased: {
            idMainRect.color = colorArray[0]
        }
        onClicked: {
            sigClick()
        }
    }
}
