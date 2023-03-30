import QtQuick 2.0
import "../HAnimation"
import "../HBaseItem"
HRectangle {
    id:idMainRect
    HImageButton{
        id:idImageButton
        width: gModel.iconSize
        height: gModel.iconSize
        mRotation:-90
        mSource:"file:./configs/HIcons/HTools/triangle.png"
        x:(idMainRect.width - width) / 2
        anchors.top: parent.bottom
        onSigClick: idMainRect.y>=0 ? hideBar() : showBar()
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
        rotate(90,-90)
        animation(-idMainRect.height,0)       
    }

    function hideBar(){
        rotate(-90,90)
        animation(0,-idMainRect.height)
    }
}
