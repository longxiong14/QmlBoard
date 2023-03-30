import QtQuick 2.0

HRectangleTextButton {
    property bool flag: false
    width: 100
    height: 30
    onSigClicked: flag = !flag
    colorArray: flag ? ["#0065E7","#92BD6C","#B0C8F9"] :
                       ["#5E5E5E","#92BD6C","#B0C8F9"]
}
