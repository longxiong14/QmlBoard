import QtQuick 2.0

Flow {
    signal sigFlush()

    property var list: []
    property int curIndex: -1
    property string curSel: ""

    HComboBox{
        id:idCombobox
        model: list
        width: parent.width - parent.implicitHeight - parent.spacing
        height: parent.height
        implicitHeight:  parent.height
        currentIndex: curIndex
        onActivated:  {
            curSel = list[currentIndex]
            curIndex = currentIndex
        }
    }

    HImageButton{
        width: 20
        height: width
        mSource: "file:./configs/HIcons/HTools/flush.png"
        onSigClick: sigFlush()
    }
}
