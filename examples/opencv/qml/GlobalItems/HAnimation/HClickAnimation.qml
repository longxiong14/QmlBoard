import QtQuick 2.0

Component {
    id: brushComp
    Item {
        id: circleItem
        property color circleColor
        property int loopsSize: 1

        Rectangle {
            id: circle
            anchors.centerIn: parent
            color: "transparent"
            width: radius * 2
            height: radius * 2
            opacity: 1
            visible: opacity > 0
            border.color: circleItem.circleColor
            border.width: 4
        }
        SequentialAnimation {
            id: animation
            running: true
            loops: loopsSize
            ParallelAnimation {
                NumberAnimation {
                    target: circle
                    property: "radius"
                    from: 1
                    to: 60
                    duration: 400
                }
                NumberAnimation {
                    target: circle
                    property: "opacity"
                    from: 1
                    to: 0
                    duration: 400
                }
            }
            ScriptAction {
                script: {
                    circleItem.destroy()
                }
            }
        }
    }
}
