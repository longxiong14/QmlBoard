import QtQuick 2.0
import QtQuick.Controls 2.5
HRectangle {
    property string tipString: ""
    property var textColor: "black"

    ToolTip{
        id:idToolTip
        text: tipString
    }

    MouseArea{
        id:idMouse
        anchors.fill: parent
        hoverEnabled: true
        onEntered: {
            idToolTip.visible = true
        }
        onExited: {
            idToolTip.visible = false
        }
    }
}
