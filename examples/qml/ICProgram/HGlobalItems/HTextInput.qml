import QtQuick 2.0

HRectangle{
    signal sigTextChanged(var txt)
    property var txt: ""
    border.color: "#eee"
    border.width: 1
    clip: true
    TextInput {
        anchors.fill: parent
        font.pixelSize: gFontSize
        text: txt
        onTextChanged: {
            sigTextChanged(text)
        }
        selectByMouse: true
    }
}

