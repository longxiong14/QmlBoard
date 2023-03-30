import QtQuick 2.0
//import QtQuick 2.13
import QtQuick.Controls 1.4
TableView  {
    id:idTableView
//    itemDelegate: idItemDelegateText
    headerDelegate: idHeaderDelegate
    rowDelegate: idRowRectangleDelegate
    Component{
        id:idItemDelegateText
        Rectangle{
            height: 30
            border.width: 1
            border.color: "#424242"
//            color: styleData.selected?"#FFDEAD":"white"
            Text {
                anchors.verticalCenter: parent.verticalCenter
                //            color: styleData.selected?"#1E90FF":"#000000"
                color: styleData.selected?"blue":"black"
                elide: styleData.elideMode
                text: styleData.value

                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
            }
        }
    }

    Component{
        id:idHeaderDelegate
        Rectangle{
            height: 30
            border.width: 1
            color: "#424242"
            //            color: "#FAEBD7"
            //            border.color: "#04f6f6"
            Text {
                id: headerName
                text: styleData.value
//                font.pointSize: 15
                font.pointSize: 12
                font.bold: true
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.fill: parent
                color: "white"
            }
        }
    }

    Component{
        id:idRowRectangleDelegate
        Rectangle{
            id:rowRectangle
            property color rowColor: idTableView.currentRow === styleData.row ?
                                         "lightblue" :  0 === styleData.row % 2 ?
                                        "white" : "#eee"
            color:rowColor
            height: 30
        }
    }

}
