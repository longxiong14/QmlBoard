import QtQuick 2.0
import "../HGlobalItems"
import hHistogramChart 1.0
HRectangle {

    HHistogramChart{
        anchors.fill: parent
        onLeftChanged: console.log("left ", left)
        onRightChanged: console.log("right ", right)
        color: "#ee0a00"
    }

}
