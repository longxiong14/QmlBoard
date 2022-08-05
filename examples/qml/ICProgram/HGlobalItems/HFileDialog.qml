import QtQuick 2.0
import QtQuick.Dialogs 1.3
FileDialog {
    property int model: 0 //0打开文件 1保存文件 2打开文件夹
    property int outModel: 0
    signal sigSelect(string txt, int model)
    signal sigOpenFile(string file)
    signal sigSaveFile(string file)
    signal sigOpenFolder(string path)
    id:idFileDialog

    onAccepted: {
        var destPath
        if(selectFolder){
            destPath = String(folder)
            destPath=destPath.substr(8)
            console.log(destPath)
        }else{
            destPath = String(fileUrl)
            destPath = destPath.substr(8)
        }
        sigSelect(destPath, model)
        if(0 === model){
            sigOpenFile(destPath)
        }else if(1 === model){
            sigSaveFile(destPath)
        }else if(2 === model){
            sigOpenFolder(destPath)
        }
    }

    function openFile(array){
        selectFolder = false
        selectExisting = true
        nameFilters = array
        model = 0
        open()
    }

    function saveFile(){
        selectFolder = false
        selectExisting = false
        nameFilters = ["所有文件 (*.*)"]
        model = 1
        open()
    }

    function openFolder(){
        selectFolder = true
        selectExisting = false
        nameFilters = []
        model = 2
        open()
    }

    function openModel(array, m){
        outModel = m
        openFile(array)
    }
}
