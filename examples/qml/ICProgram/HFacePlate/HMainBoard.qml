import "../HGlobalItems"
import hBoard 1.0
import QtQuick 2.13
HRectangle{
    property var boardName: "main_board"
    property var items: []
    id: idMainRectangle
    Flow{
        width: parent.width
        height: parent.height
        HBoard{
            id:idBoard
            width: parent.width
            height: idMainRectangle.height - idHoverd.implicitHeight
            onHoverPoint: {
                idHoverd.text = "x: " + x + " y:" + y
            }
            name: boardName
            onItemsChanged: idMainRectangle.items = idUIControl.paramToUIItems(items)
        }
        HText {
            id: idHoverd
        }
    }
}

