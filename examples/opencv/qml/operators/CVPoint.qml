import QtQuick 2.13

Row{
    property var cvPoint: ({point_x:0,
                           point_y:0})
    spacing: 10
    Text {
        width: 60
        text: qsTr("point")
    }

    Column{
        spacing: 10
        Row{
            Text{
                width: 30
                horizontalAlignment: Text.AlignRight
                text: qsTr("x")
            }

            Rectangle{
                border.color: "#eee"
                border.width: 2
                width: 100
                height: 18
                TextInput{
                    text: qsTr("0")
                    anchors.fill: parent
                    anchors.margins:2
                    selectByMouse:true
                    validator: IntValidator{}
                    onTextEdited:{
                        cvPoint["point_x"] = Number(text)
                    }
                }
            }
        }

        Row{
            Text{
                width: 30
                horizontalAlignment: Text.AlignRight
                text: qsTr("y")
            }
            Rectangle{
                border.color: "#eee"
                border.width: 2
                width: 100
                height: 18
                TextInput{
                    text: qsTr("0")
                    anchors.fill: parent
                    anchors.margins:2
                    selectByMouse:true
                    validator: IntValidator{}
                    onTextEdited:{
                        cvPoint["point_y"] = Number(text)
                    }
                }
            }
        }
    }
}
