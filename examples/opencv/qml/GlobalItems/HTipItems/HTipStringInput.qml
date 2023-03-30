import QtQuick 2.0
import "../HBaseItem"
HFlow {
    property int tipWidth: 100
    property var tipStr: ""
    property var initVal: ""
    property var curVal: ""
    property int subWidth: 0
    property bool canEdit: true
    property var bColor: "transparent"
    property var tColor: gModel.fontColor
    HText{
        width: tipWidth
        height: parent.height
        text: tipStr
        horizontalAlignment: Text.AlignRight
        color: tColor
    }
    HRectangle{
        border.color: "#eee"
        border.width: 1
        width: parent.width - tipWidth - parent.spacing - subWidth
        height: parent.height
        clip: true
        color: bColor
        TextInput {
            id:idInput
            anchors.fill: parent
            font.pixelSize: gModel.fontSize
            enabled: canEdit
            color: tColor
            onTextChanged: {
                curVal = text
            }
            selectByMouse: true
        }        
    }
    Component.onCompleted: setValue(initVal)
    function setValue(v){
        idInput.text = v
    }
}
