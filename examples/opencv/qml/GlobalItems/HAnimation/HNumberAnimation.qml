import QtQuick 2.0
import QtQuick 2.13
Component{
    SequentialAnimation {
        id: animation
        running: true
        loops: 1
        property var target
        property var from
        property var to
        property var pp

        NumberAnimation{
            property: animation.pp
            duration: 250
            target: animation.target
            from: animation.from
            to:animation.to
        }

//        NumberAnimation{
//            property: animation.pp
//            duration: 50
//            target: animation.target
//            from: animation.to
//            to:animation.from / 5
//        }

//        NumberAnimation{
//            property: animation.pp
//            duration: 50
//            target: animation.target
//            from: animation.from / 5
//            to:animation.to
//        }

//        NumberAnimation{
//            property: animation.pp
//            duration: 25
//            target: animation.target
//            from: animation.to
//            to:animation.from / 10
//        }

//        NumberAnimation{
//            property: animation.pp
//            duration: 25
//            target: animation.target
//            from: animation.from / 10
//            to:animation.to
//        }

        ScriptAction {
            script: {
                animation.destroy()
            }
        }
    }
}
