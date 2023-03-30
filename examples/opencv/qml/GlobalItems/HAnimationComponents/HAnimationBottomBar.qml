import QtQuick 2.0
import "../HAnimation"
import "../HBaseItem"
HRectangle {
    id:idMainRect
    y:parent.height-height
    HImageButton{
        id:idImageButton
        width: gModel.iconSize
        height: gModel.iconSize
        mRotation: 90
        mSource:"file:./configs/HIcons/HTools/triangle.png"
        x:(idMainRect.width - width) / 2
        anchors.bottom: parent.top
        onSigClick: idMainRect.y !== idMainRect.parent.height ? hideBar() : showBar()
    }

    HNumberAnimation{
        id:idNumberAnimation
    }

    function animation(f,t){
        idNumberAnimation.createObject(idMainRect,{target:idMainRect,from:f,to:t,pp:"y"})
    }
    function rotate(f,t){
        idNumberAnimation.createObject(idMainRect,{target:idImageButton,from:f,to:t,pp:"mRotation"})
    }

    function showBar(){
        rotate(-90,90)
        animation(idMainRect.parent.height,idMainRect.parent.height-height)
    }

    function hideBar(){
        rotate(90,-90)
        animation(idMainRect.parent.height-height,idMainRect.parent.height)
    }
}
