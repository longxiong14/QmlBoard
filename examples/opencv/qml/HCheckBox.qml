import QtQuick 2.9
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
CheckBox {
    property var fontSize: 16
    property var textColor: "black"
    property var isfontbold: false

    id:idControl
    style: CheckBoxStyle {
        indicator: Rectangle {
            implicitWidth: 16
            implicitHeight: 16
            radius: 3
            border.color: control.activeFocus ? "darkblue" : "gray"
            border.width: 1
            Rectangle {
                visible: control.checked
                color: "#555"
                border.color: "#333"
                radius: 1
                anchors.margins: 4
                anchors.fill: parent
            }
        }
        label: Text {
            text: idControl.text
            font.pixelSize: fontSize
            color: textColor
            font.bold: isfontbold
        }
    }
}
