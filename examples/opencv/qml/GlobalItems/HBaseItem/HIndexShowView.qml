import QtQuick 2.0
import "../HAnimation"
HRectangle {
    id:idIndexMainRectangle
    property var currentIndex: 0
    onCurrentIndexChanged: {
        showIndex(currentIndex)
    }
    Component.onCompleted: {
        showIndex(currentIndex)
    }
    function showIndex(index){
        let size = children.length
        for(var i = 0; i < size; i++){
            let c = children[i]
            if(c.visible){
                c.visible = false
            }
        }
        if(index < size && index >= 0){
            let t = children[index]
            idNumber.createObject(idIndexMainRectangle,{target:t,from:idIndexMainRectangle.width,to:0,pp:"x"})
            t.visible = true
        }
    }

    HNumberAnimation{
        id:idNumber
    }
}
