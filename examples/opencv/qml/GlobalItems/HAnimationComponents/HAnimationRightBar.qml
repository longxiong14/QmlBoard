import QtQuick 2.0
import "../HAnimation"
import "../HBaseItem"
HRectangle {
    id:idMainRect
    x:parent.width-width
    HImageButton{
        id:idImageButton
        width: gModel.iconSize
        height: gModel.iconSize
        mSource:"file:./configs/HIcons/HTools/triangle.png"
        y:(idMainRect.height - height) / 2
        anchors.right: parent.left
        onSigClick: idMainRect.x !== idMainRect.parent.width ? hideBar() : showBar()
    }

    HNumberAnimation{
        id:idNumberAnimation
    }

    function animation(f,t){
        idNumberAnimation.createObject(idMainRect,{target:idMainRect,from:f,to:t,pp:"x"})
    }
    function rotate(f,t){
        idNumberAnimation.createObject(idMainRect,{target:idImageButton,from:f,to:t,pp:"mRotation"})
    }

    function showBar(){
        rotate(180,0)
        animation(idMainRect.parent.width,idMainRect.parent.width-width)
    }

    function hideBar(){
        rotate(0,180)
        animation(idMainRect.parent.width-width,idMainRect.parent.width)
    }
}
