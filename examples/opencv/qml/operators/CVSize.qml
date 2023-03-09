import QtQuick 2.13

Row{
    property var cvSize: ({size_width:0,
                          size_height:0})
    spacing: 10
    Text {
        width: 60
        text: qsTr("size")
    }

    Column{
        spacing: 10
        Row{
            Text{
                width: 30
                horizontalAlignment: Text.AlignRight
                text: qsTr("width")
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
                        cvSize["size_width"] = Number(text)
                    }
                }
            }
        }

        Row{
            Text{
                width: 30
                horizontalAlignment: Text.AlignRight
                text: qsTr("height")
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
                        cvSize["size_height"] = Number(text)
                    }
                }
            }
        }
    }
}
