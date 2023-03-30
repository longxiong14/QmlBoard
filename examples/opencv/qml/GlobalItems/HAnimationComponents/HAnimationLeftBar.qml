import QtQuick 2.0
import "../HAnimation"
import "../HBaseItem"
HRectangle {
    id:idMainRect
    HImageButton{
        id:idImageButton
        width: gModel.iconSize
        height: gModel.iconSize
        mRotation:180
        mSource:"file:./configs/HIcons/HTools/triangle.png"
        y:(idMainRect.height - height) / 2
        anchors.left: parent.right
         onSigClick: idMainRect.x>=0 ? hideBar() : showBar()
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
        rotate(0,180)
        animation(-idMainRect.width,0)
    }

    function hideBar(){
        rotate(180,0)
        animation(0,-idMainRect.width)
    }
}
