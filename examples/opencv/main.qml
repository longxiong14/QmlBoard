import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13
import QtQuick.Controls 1.4
import hBoard 1.0
import hUIControl 1.0
import "./qml"
import "./qml/tabs"
import hEnumModel 1.0
ApplicationWindow {
    visible: true
    title: qsTr("opencv flow")
    width: 1900
    height: 1040
    onVisibleChanged: if(visible) showMaximized()

    menuBar: MenuBar {
        Menu {
            title:qsTr("open")
            MenuItem {
                text: qsTr("open")
                onTriggered: {
                    idFile.open()
                }
            }
        }
    }

    ExclusiveGroup{
        id:idEx
    }

    SplitView{
        width: parent.width
        height: parent.height
        GridLayout{
            id:idGridLayout
            width: 1450
            height: 965
            columns: 3
            rows: 2
            rowSpacing: 5
            columnSpacing: 5

            Repeater{
                id:idRepeater
                property int showIndex: -1
                model: 6
                delegate: Rectangle{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    visible: -1 === idRepeater.showIndex ?
                                 true : idRepeater.showIndex === index
                    border.width: 1
                    border.color: "red"
                    radius: 2
                    HBoard{
                        id:idBoard
                        anchors.fill: parent
                        rule: false
                        name: String(index)
                        Component.onCompleted: idUIControl.setBoardHandle(name,"mult_handle")
                    }
                    Column{
                        spacing: 10
                        HCheckBox{
                            text: qsTr("source")
                            onClicked: {
                                gModel.checkedSource(idBoard.name, checked)
                            }
                            Component.onCompleted: if(0 === index) {
                                                       gModel.checkedSource(idBoard.name, true)
                                                       checked = true
                                                   }
                        }
                        HCheckBox{
                            text: qsTr("result")
                            exclusiveGroup:idEx
                            checked: gModel.dest === idBoard.name
                            onClicked: gModel.dest = idBoard.name
                        }
                    }
                    Button{
                        anchors.right: parent.right
                        width: 30
                        height: 30
                        onClicked: idRepeater.showIndex =
                                   -1 === idRepeater.showIndex ? index : -1
                    }
                    Text {
                        anchors.bottom: parent.bottom
                        text: qsTr(idBoard.msg)
                    }
                }
            }
        }

        Rectangle{
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: "#eee"
            border.width: 2
            TabView{
                anchors.fill: parent
                anchors.margins: 5
                HBlurredTab{

                }
                HAffineTransformationTab{

                }
            }
        }
    }

    FileDialog{
        id:idFile
        selectFolder : false
        selectExisting : true
        nameFilters : ["png (*.png)", "jpg (*.jpn)", "bmp (*.bmp)", "all (*.*)"]
        onAccepted: {
            let destPath = String(fileUrl)
            destPath = destPath.substr(8)
            gCtrl.openImage(destPath)
        }
    }

    HUIControl{
        id:idUIControl
    }

    HEnumModel{
        id:idEnumModel
    }
}
