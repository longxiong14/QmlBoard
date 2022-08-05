import QtQuick 2.0
import QtQuick.Window 2.13

Window {
    modality:Qt.WindowModal
    HRectangle{
        id:idTitle
        width: parent.width
        color: "red"
        height: 30
        HText{
            text: "错误！"
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
            anchors.centerIn: parent
        }
    }

    function showError(error){
        idText.text = error
        show()
    }
}
