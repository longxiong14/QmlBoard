import QtQuick 2.12
import QtQuick.Controls 2.5

ComboBox {
    id: control
    font.pixelSize: gModel.fontSize
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: modelData
            color: control.highlightedIndex === index ?
                       "#21be2b" : "black"
            font: control.font
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
            height: control.height
        }
        highlighted: control.highlightedIndex === index
    }

    popup: Popup {
        y: control.height - 1
        width: control.width
        implicitHeight: contentItem.implicitHeight
        padding: 1

        contentItem: ListView {
            clip: true
            implicitHeight: contentHeight
            model: control.popup.visible ? control.delegateModel : null
            currentIndex: control.highlightedIndex
            ScrollIndicator.vertical: ScrollIndicator { }
        }

        background: Rectangle {
            border.width: 1
            radius: 2
        }
    }
    onModelChanged: currentIndex = -1
    property bool init: false

    function setCurrentText(txt){
        var model = control.model
        for(var i = 0 ; i < model.length ;i++){
            var modelTxt = model[i]
            if(modelTxt === txt){
                control.currentIndex = i
                continue
            }
        }
    }

}
