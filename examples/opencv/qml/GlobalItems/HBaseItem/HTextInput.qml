import QtQuick 2.0

HRectangle{
    signal sigTextChanged()
    property var txt: ""
    border.color: "#eee"
    border.width: 1
    clip: true
    TextInput {
        property bool is_init: false
        anchors.fill: parent
        font.pixelSize: gModel.fontSize
        onTextChanged: {
            if(is_init){
                txt = text
                sigTextChanged()
            }
        }
        Component.onCompleted: {
            text = txt
            is_init = true
        }
        selectByMouse: true
    }
}

