import QtQuick 2.0
import "../HBaseItem"
HFlow {
    property int tipWidth: 100
    property var tipStr: ""
    property var initVal: 0
    property var curVal: initVal
    property var bColor: "transparent"
    HText{
        width: tipWidth
        height: parent.height
        text: tipStr
        horizontalAlignment: Text.AlignRight
    }
    HRectangle{
        border.color: "#eee"
        border.width: 1
        width: parent.width - tipWidth - parent.spacing
        height: parent.height
        clip: true
        color: bColor
        TextInput {
            anchors.fill: parent
            font.pixelSize: gModel.fontSize
            color: "black"
            text: String(initVal)            
            onTextChanged: {
                curVal = Number(text)
            }
            validator: DoubleValidator{
            }
            selectByMouse: true
        }
    }
}
