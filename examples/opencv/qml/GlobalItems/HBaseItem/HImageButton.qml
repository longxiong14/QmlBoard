import QtQuick 2.0
import QtQuick.Controls 2.5
HRectangle{
    property string backColor: "transparent"
    property var colorArray: ["transparent", "#00a2e8"]
    property string mSource: ""
    property int mRotation: 0
    property double mOpacity: 0.7
    property string toolTipString: ""
    property int mMargins: 1
    signal sigLongPress()
    signal sigClick()
    id:idMainRect
    color: colorArray[0]
    border.width: 1
    border.color: "#eee"
    ToolTip {
        id:idToolTip
        visible: false
        text: toolTipString
        delay: 100
    }

    Image {
        anchors{
            fill:parent
            margins: mMargins
        }
        id: idUpButton
        source: mSource
        height: parent.height
        fillMode: Image.PreserveAspectFit
        opacity: mOpacity
        anchors.centerIn: parent
        rotation: mRotation
    }



    MouseArea{
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            idMainRect.color = Qt.rgba(145/255,201/255,247/255)
            if(0 !== toolTipString.length)
                idToolTip.visible = true
        }
        onExited: {
            idMainRect.color = colorArray[0]
            idToolTip.visible = false
        }
        onPressed: {
            idMainRect.color = colorArray[1]
        }
        onReleased: {
            idMainRect.color = colorArray[0]
        }
        onClicked: {
            sigClick()
        }
    }
    onBackColorChanged: {
        colorArray = [ backColor, "#00a2e8"]
        color = colorArray[0]
    }
    onColorArrayChanged: color = colorArray[0]
}
