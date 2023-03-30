import QtQuick 2.0
import "../HBaseItem"
HFlow {
    property var tipStr: ""
    property double initValue: 0
    property double curValue: 0
    property double stepValue: 1
    property double maxValue: 255
    property double minValue: 0
    property bool init : false
    spacing: 5
    HText{
        id:idTip
        text: tipStr
    }

    HText{
        id:idSubstract
        text: "-"
        color: "red"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                idSlider.value -= idSlider.stepSize
            }
            hoverEnabled: true
            onEntered: cursorShape = Qt.PointingHandCursor
            onExited: cursorShape = Qt.ArrowCursor
        }
    }

    HSliderBlock{
        id:idSlider
        width: parent.width -
               idTip.width -
               idValueText.width -
               idSubstract.implicitWidth -
               idAdd.implicitWidth -
               parent.spacing * 4
        from: minValue
        to: maxValue
        stepSize: stepValue       
        live: false
        onValueChanged: {
            curValue = value
        }
    }

    HText{
        id:idAdd
        text: "+"
        color: "blue"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                idSlider.value += idSlider.stepSize
            }
            hoverEnabled: true
            onEntered: cursorShape = Qt.PointingHandCursor
            onExited: cursorShape = Qt.ArrowCursor
        }
    }

    HText{
        id:idValueText
        width: 30
        text: String(idSlider.value)
    }
    Component.onCompleted: {
        setValue(initValue)
    }

    function setValue(v){
        idSlider.value = v
    }
}
