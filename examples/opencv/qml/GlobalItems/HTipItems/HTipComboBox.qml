import QtQuick 2.0
import "../HBaseItem"
HFlow{
    property var txt: ""
    property var list: []
    property var curSel: ""
    property var curIndex: -1
    property var tipWidth: 100
    HText{
        id:idTip
        text: txt
        width: tipWidth
        horizontalAlignment: Text.AlignRight
    }
    HComboBox{
        id:idCombobox
        model: list
        width: parent.width -  idTip.width - parent.spacing
        height: parent.height
        implicitHeight:  parent.height
        currentIndex: curIndex
        onActivated:  {
            curSel = list[currentIndex]
            curIndex = currentIndex
        }
    }

    function setCurSel(t){
        var size = idCombobox.model.length
        for(var i = 0; i < size; i++){
            if(t === idCombobox.model[i]){
                idCombobox.currentIndex = i
                curSel = t
                break
            }
        }
    }

    function setCurIndex(i){
        idCombobox.currentIndex = i
        curIndex = i
        curSel = list[i]
    }


    Component.onCompleted: setCurSel(curSel)
}
