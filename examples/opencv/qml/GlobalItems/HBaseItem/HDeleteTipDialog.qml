import QtQuick 2.0

HWindow {
    signal sigDelete()
    width: 300
    height: 200
    color: gModel.backGround
    title: qsTr("warning") + trans.transString
    Text {
        font.pixelSize: gModel.fontSize
        color: gModel.fontColor
        text: qsTr("delete %1 ?").arg(tip) + trans.transString
        anchors.centerIn: parent
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
                sigDelete()
            }
        }
        HRectangleTextButton{
            width: 100
            height: 30
            txt: qsTr("cancle") + trans.transString
            onSigClicked: close()
        }
    }

    property var tip: ""
    function isDelete(t){
        tip = t
        show()
    }
}
