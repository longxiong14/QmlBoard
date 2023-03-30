import QtQuick 2.0

HRectangle{
    signal sigNumberChanged()
    property double txt: 0
    border.color: "#eee"
    border.width: 1
    clip: true
    TextInput {
        property bool is_init: false
        anchors.fill: parent
        font.pixelSize: gModel.fontSize
        validator: DoubleValidator{

        }
        onTextChanged: {
            if(is_init){
                txt = Number(text)
                sigNumberChanged()
            }
        }
        Component.onCompleted: {
            text = txt
            is_init = true
        }
        selectByMouse: true
    }
}

