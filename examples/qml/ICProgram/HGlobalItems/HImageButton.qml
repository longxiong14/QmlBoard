import QtQuick 2.0
import QtQuick.Controls 2.5
HRectangle{
    property string backColor: "transparent"
    property string mSource: "resources/trigle.png"
    property int mRotation: 0
    property double mOpacity: 0.7
    property string toolTipString: ""
    property int mMargins: 2
    signal sigClick()
    signal sigLongPress()
    id:idMainRect
    color: backColor
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
            idMainRect.color = backColor
            idToolTip.visible = false
        }
        onPressed: {
            idMainRect.color = "#00a2e8"
            idLongPressTimer.start()
        }
        onReleased: {
            idMainRect.color = backColor
            idLongPressTimer.stop()
        }
        onClicked: {
            sigClick()
        }
    }

    Timer{
        id:idLongPressTimer
        running: false
        interval: 100
        repeat: true
        onTriggered: {
            sigLongPress();
        }
    }
    onBackColorChanged: color = backColor
}
