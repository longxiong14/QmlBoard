import QtQuick 2.0
import "../HBaseItem"
HFlow{
    signal sigFlush()

    property var txt: ""
    property var list: []
    property var curSel: ""
    property var curIndex: -1
    HText{
        id:idTip
        text: txt
    }

    HFMComboBox{
        width: parent.width - parent.spacing - idTip.implicitWidth
        height: parent.height
        onCurIndexChanged: parent.curIndex = curIndex
        onCurSelChanged: parent.curSel = curSel
        list: parent.list
        onSigFlush: parent.flush()
    }

    function flush(){
        sigFlush()
    }
}
