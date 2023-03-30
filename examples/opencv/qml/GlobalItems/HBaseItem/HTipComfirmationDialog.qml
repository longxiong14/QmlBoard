import QtQuick 2.0

HWindow {
    property var tip: ""
    signal sigSure()
    width: 300
    height: 200
    color: gModel.backGround
    title: qsTr("warning") + trans.transString
    HText{
        anchors.centerIn: parent
        text: tip
    }

    Row{
        spacing: 5
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        HRectangleTextButton{
            width: 100
            height: 30
            txt: qsTr("sure") + trans.transString
            onSigClicked: {
                close()
                sigSure()
            }
        }
        HRectangleTextButton{
            width: 100
            height: 30
            txt: qsTr("cancle") + trans.transString
            onSigClicked: close()
        }
    }

    function showTip(s){
        tip = s
        show()
    }
}
