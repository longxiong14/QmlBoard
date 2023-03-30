import QtQuick 2.9
import QtQuick.Controls 2.2
import "../QBaseItem"

ListView {
    id: cusListView

    signal pressed(real mouseX, real mouseY)
    signal released()
    signal positionChanged(real mouseX, real mouseY)
    signal doubleClicked(real mouseX, real mouseY)

    focus: true
    clip: true
    interactive: false
    snapMode: ListView.SnapToItem
    cacheBuffer: 12000

    ScrollBar.horizontal: CusScrollBar {
        id: hbar
        opacity: cusListView.contentWidth > cusListView.width ? 1.0 : 0.0
        visible: opacity > 0
        active: visible
    }
    ScrollBar.vertical: CusScrollBar {
        id: vBar
        opacity: cusListView.contentHeight > cusListView.height ? 1.0 : 0.0
        visible: opacity > 0
        active: visible
    }

    QTransArea {
        anchors.fill: parent
        onWheel: {
            //angle
            if (vBar.visible) {
                if (wheel.angleDelta.y > 0) {
                    vBar.decrease()
                } else if (wheel.angleDelta.y < 0) {
                    vBar.increase()
                }
            }
        }
//        onEntered: {
//            mouse.accepted = false
//        }
    }
}
