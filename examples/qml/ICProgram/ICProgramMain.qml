import QtQuick 2.12
import QtQuick.Controls 2.5
import QtQuick.Controls 1.4
import "./HGlobalItems"
import "./HFacePlate"
import QtQuick.Layouts 1.13
//import hBoardView 1.0
import hUIControl 1.0
ApplicationWindow {
    visible: true
    width: 960
    height: 540
    title: qsTr("ICProgram")
    property var gBKColor: "lightblue"
    property var gFontSize: 15
    property var gFontColor: "black"

    color: gBKColor
    SplitView{
        anchors.fill: parent
        orientation: Qt.Vertical
        HGlobalButtonsArea{
            Layout.fillWidth: true
            Layout.minimumHeight: 50
            Layout.maximumHeight: 50
        }
        HRectangle{
            SplitView{
                anchors.fill: parent
                HMainBoard{
                    id:idMainBoard
                    width: 500
                    height: parent.height
                    Layout.fillWidth: true
                }
                HRectangle{
                    width: 500
                    Layout.minimumWidth: 500
                    SplitView{
                        anchors.fill: parent
                        orientation: Qt.Vertical
                        HFunctionalRegions{
                            width: parent.width
                            height: 300
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }
                        HRectangle{
                            width: 300
                            height: 300
                            SplitView{
                                anchors.fill: parent
                                HScanControl{
                                    width: 300
                                }
                                HMachineStatus{
                                    Layout.fillWidth: true
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    HUIControl{
        id:idUIControl
    }

    HErrorBox{
        id:idErrorBox
    }
}
