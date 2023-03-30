import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
TabView {
    property var titleHeight: 20
    style: TabViewStyle {
        frameOverlap: 1
        tab: Rectangle {
            color: styleData.selected ? "steelblue" :"lightsteelblue"
            border.color:  "steelblue"
            implicitWidth: Math.max(text.width + 4, 80)
            implicitHeight: titleHeight
            radius: 2
            Text {
                id: text
                anchors.centerIn: parent
                text: styleData.title
                color: styleData.selected ? "white" : "black"
            }
        }
        frame: Rectangle {
            color: "transparent"
        }
//        tabBar: TabBar {
//            id: control
//            anchors.right: parent.right
//            contentItem: ListView {
//                model: control.contentModel
//                currentIndex: control.currentIndex
//                spacing: control.spacing
//                orientation: ListView.Vertical   // <<-- VERTICAL
//                boundsBehavior: Flickable.StopAtBounds
//                flickableDirection: Flickable.AutoFlickIfNeeded
//                snapMode: ListView.SnapToItem

//                highlightMoveDuration: 0
//                highlightRangeMode: ListView.ApplyRange
//                preferredHighlightBegin: 40
//                preferredHighlightEnd: height - 40
//            }
//        }
    }
}
