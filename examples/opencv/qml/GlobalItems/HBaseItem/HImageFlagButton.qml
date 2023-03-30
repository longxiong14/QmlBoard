import QtQuick 2.0
HImageButton{
    property var flag: true
    width: 40
    height: 40
    mSource:"file:./configs/HIcons/HTools/Charts/magic_wand.png"
    backColor: flag ? "lightblue" : "transparent"
    toolTipString: qsTr("find contours") + trans.transString
    onSigClick: {
        flag = !flag
    }
}
