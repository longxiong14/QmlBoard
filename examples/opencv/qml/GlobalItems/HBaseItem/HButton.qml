import QtQuick 2.9
import QtQuick.Controls 2.2
import "../../GlobalItems/QBaseItem"

Button {
    id: cusButton
    height: 30

    property bool selected: false
    property color backgroundColorNormal: "#f5f5f6"
    property color backgroundColorHovered: Qt.darker("#ec4141", 1.2)
    property color backgroundColorPressed: Qt.darker("#ec4141", 1.4)
    property color backgroundColorDisable: Qt.lighter("#f5f5f6", 2.0)
    property color borderColor: {
        if (!cusButton.enabled) {
            return Qt.lighter("#f5f5f6", 2.0)
        } else if (cusButton.pressed || selected) {
            return Qt.darker("#ec4141", 1.4)
        } else if (cusButton.hovered) {
            return Qt.darker("#ec4141", 1.2)
        } else {
            return "#cbcbcb"
        }
    }
    property int borderWidth: 1
    property int radius: 4
    property color textColor: {
        if (!cusButton.enabled) {
            return "#9f9fcf"
        } else if (cusButton.pressed || selected) {
            return "#212121"
        } else if (cusButton.hovered) {
            return "#686868"
        } else {
            return "#282828"
        }
    }
    property color backgroundColor: {
        if (!cusButton.enabled) {
            return backgroundColorDisable
        } else if (cusButton.pressed || selected) {
            return backgroundColorPressed
        } else if (cusButton.hovered) {
            return backgroundColorHovered
        } else {
            return backgroundColorNormal
        }
    }

//    contentItem: Text {
//        text: cusButton.text
//        color: cusButton.textColor
//    }

    background: Rectangle {
        radius: cusButton.radius
        color: cusButton.backgroundColor
        border.color: cusButton.borderColor
        border.width: cusButton.borderWidth
    }

    QTransArea {
    }
}
