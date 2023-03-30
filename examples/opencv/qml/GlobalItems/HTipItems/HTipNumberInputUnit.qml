import QtQuick 2.0
import "../HBaseItem"
Row {
    property int tipWidth: 100
    property int inputWidth: 100
    property int unitWidth: 30
    property var unitStr: ""
    property var tipStr: ""
    property var initVal: 0
    property var curVal: ""
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
        width: inputWidth
        height: parent.height
        clip: true
        color: bColor
        TextInput {
            anchors.fill: parent
            font.pixelSize: gModel.fontSize
            text: String(initVal)
            onTextChanged: {
                curVal = Number(text)
            }
            validator: DoubleValidator{
            }
            selectByMouse: true
        }
    }

    HText{
        width: unitWidth
        height: parent.height
        text: unitStr
        horizontalAlignment: Text.AlignLeft
    }
}
