import QtQuick 2.0
import "../HGlobalItems"
HRectangle {

    Flow{
        spacing: 5
        anchors.fill: parent
        anchors.margins: 5
        HButton{
            text: "打开图片"
            onClicked: {
                idFileDialog.openFile(["png文件 (*.png)", "所有文件 (*.*)"])
            }
        }
        HButton{
            text: "测试"
            visible: false
            onClicked: {
                idErrorBox.showError("错误")
            }
        }
    }

    HFileDialog{
        id:idFileDialog
        onSigOpenFile: {
            idUIControl.openBoardPicture(idMainBoard.boardName, file)
        }
    }
}
