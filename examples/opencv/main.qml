import QtQuick 2.12
import QtQuick.Dialogs 1.3
import QtQuick.Layouts 1.13
import QtQuick.Controls 1.4
import hBoard 1.0
import hUIControl 1.0
import "./qml"
ApplicationWindow {
    visible: true
    title: qsTr("opencv flow")
    onVisibleChanged: if(visible) showMaximized()

    ExclusiveGroup{
        id:idEx
    }

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
                }
                Column{
                    spacing: 10
                    HCheckBox{
                        text: qsTr("source")
                        onClicked: {
                            gModel.checkedSource(idBoard.name, checked)
                            console.log(JSON.stringify(gModel.sourceChecked))
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
            }
        }
    }
}
