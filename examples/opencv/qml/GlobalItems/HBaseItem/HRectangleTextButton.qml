import QtQuick 2.0
import "../QBaseItem"

HRectangle {
    signal sigClicked()
    signal sigDoubleClicked()
    signal sigPressed()
    signal sigReleased()

    id: idRoot
    property var txt : ""
    property var colorArray: ["#5E5E5E","#92BD6C","#B0C8F9"]
    property var alignment: Text.AlignLeft
    property var txtLeftMargin: 10
    property var fontSize: gModel.fontSize
    property var fontBold: false
    property var txtColor: "white"
    color: colorArray[0]
    radius: 5
    HText{
        id:idText
        horizontalAlignment: alignment
        verticalAlignment: Text.AlignVCenter
        text: qsTr(txt) + trans.transString
        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
        anchors.fill: parent
        anchors.leftMargin: txtLeftMargin
        font.pixelSize: fontSize
        font.bold: fontBold
        color: idRoot.enabled ? txtColor : "gray"
    }
    MouseArea{
        anchors.fill: parent
        onClicked: sigClicked()
        hoverEnabled: true
        onExited:  parent.color = colorArray[0]
        onEntered: parent.color = colorArray[1]
        onPressed: {
            sigPressed()
            parent.color = colorArray[2]
        }
        onDoubleClicked: sigDoubleClicked()
        onReleased: sigReleased()
        //onReleased: parent.color = colorArray[1]
    }
    onColorArrayChanged:{
        color = colorArray[0]
    }

}
