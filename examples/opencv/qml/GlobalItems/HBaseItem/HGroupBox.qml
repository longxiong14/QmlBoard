import QtQuick 2.0
import QtQuick.Controls 2.5
import QtGraphicalEffects 1.0
GroupBox {
    property var radiusVal: 3                     // 圆角值
    property var borderWidth:  1                  // 边框宽度
    property var borderColor: "#97C4F5"           // 边框颜色
    property var titleColor:  "#2359B7"           // 标题颜色
    property var titleLeftBkColor:  "#C9DDF8"     // 标题最左侧背景色
    property var titleRightBkColor:  "#F7FAFF"    // 标题最右侧颜色
    property var titleTopPadding:  3              // 标题的顶部内边距
    property var contentBkColor:  "#77F0F6FF"     // 内容背景色
    property var titleFontPixel:  16

    id: control
    title: qsTr("GroupBox")

    background: Rectangle {
        anchors.fill: control
        radius: radiusVal
        border.color: borderColor
        color: contentBkColor
        clip: true
        Item {
            x: borderWidth
            y: borderWidth
            width: parent.width - borderWidth * 2
            height: control.topPadding - control.bottomPadding - borderWidth * 2 + titleTopPadding * 2
            clip: true

            Rectangle {
                anchors.fill: parent
                color: "#228833"

                LinearGradient {
                    anchors.fill: parent
                    source: parent
                    start: Qt.point(0, 0)
                    end: Qt.point(parent.width, 0)
                    gradient: Gradient {
                        GradientStop { position: 0.0; color: titleLeftBkColor }
                        GradientStop { position: 1.0; color: titleRightBkColor }
                    }
                }
            }
        }
    }

    label: Label {
        y:  titleTopPadding + 3
        x: control.leftPadding
        width: control.availableWidth
        text: control.title
        color: titleColor
        elide: Text.ElideRight
        font.family: "Microsoft Yahei"
        font.pixelSize: titleFontPixel
    }
}
