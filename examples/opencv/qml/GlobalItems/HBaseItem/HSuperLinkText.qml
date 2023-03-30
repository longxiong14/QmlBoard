import QtQuick 2.0

Text {
    signal sigClicked()
    color: "blue"
    MouseArea{
        anchors.fill: parent
        onClicked: sigClicked()
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
    }
}
