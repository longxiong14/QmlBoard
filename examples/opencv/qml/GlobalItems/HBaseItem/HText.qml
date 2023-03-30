import QtQuick 2.0
import QtQuick.Controls 2.5

Text {
    font.pixelSize: gModel.fontSize
    color: gModel.fontColor
    //font.family: gModel.fontFamily
    property var tip: ""

    MouseArea{
        visible: tip.length !== 0
        hoverEnabled: true
        anchors.fill: parent
        onExited: idToolTip.visible = false
        onEntered: idToolTip.visible = true
    }

    ToolTip{
        id:idToolTip
        text: tip
    }
}
