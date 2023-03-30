import QtQuick 2.0
import QtQuick.Window 2.13

Window {
    property var vTitle: "error"
    property var vColor: "red"
    modality:Qt.WindowModal
    color: gModel.backGround
    HRectangle{
        id:idTitle
        width: parent.width
        color: vColor
        height: 30
        HText{
            text: vTitle
            wrapMode: Text.WrapAnywhere
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    HRectangle{
        anchors{
            top: idTitle.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        HText{
            id:idText
            wrapMode: Text.WrapAnywhere
            width: parent.width
            anchors.centerIn: parent
        }
    }

    function showError(error){
        idText.text = error
        show()
    }
}
