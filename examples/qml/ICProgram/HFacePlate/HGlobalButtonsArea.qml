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
                idFileDialog.openModel(["png文件 (*.png)", "所有文件 (*.*)"], 0)
            }
        }
        HButton{
            text: "保存"
            onClicked: {
                idFileDialog.saveFile()
            }
        }

        HButton{
            text: "加载"
            onClicked: {
                idFileDialog.openModel(["所有文件 (*.*)"], 1)
            }
        }

        HButton{
            text: "测试"
            onClicked: {
                idUIControl.test()
            }
        }
    }

    HFileDialog{
        id:idFileDialog
        onSigOpenFile: {
            switch(outModel){
            case 0:
                idUIControl.openBoardPicture(idMainBoard.boardName, file)
                break
            case 1:
                idUIControl.loadBoard(idMainBoard.boardName, file)
                break
            }
        }
        onSigSaveFile: {
            idUIControl.saveBoard(idMainBoard.boardName, file)
        }
    }
}
