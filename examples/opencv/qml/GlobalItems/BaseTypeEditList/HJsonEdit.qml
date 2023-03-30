import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13

ScrollView{
    property int colHeight: 30
    property int leftWidth: 100
    property int itemWidth: 300
    property var mModel: []
    property int textSize: 18
    property var tipColor: "transparent" //"#4963ff"
    property var backgroundColor: "transparent"
    property var tipTxtColor: "black"
    property var colorArray: ["red","transparent"]
    property var flowSpacing: 5
    property var hasChanged: false

    signal sigValueChanged()
    spacing: 5

    onMModelChanged: {
        if(idSourceLoader.item){
            idSourceLoader.item.setDefault(mModel)
            hasChanged = false
        }
    }

    Loader{
        id:idSourceLoader
        sourceComponent: idSourceComponent
        Component.onCompleted: {
            item.setDefault(mModel)
        }
    }

    Connections{
        target: idSourceLoader.item
        onSigChanged: function(){
            hasChanged = true
            sigValueChanged()
        }
    }

    Component{
        id: idSourceComponent
        Column{
            signal sigChanged()
            spacing: flowSpacing
            Repeater{
                property var mModel: []
                id:idRepeater
                model: idRepeater.mModel
                delegate: Row {
                    spacing: flowSpacing
                    Rectangle{
                        width: leftWidth
                        height: colHeight
                        color: tipColor
                        radius: 4
                        clip: true
                        Text {
                            id:idTitle
                            color: tipTxtColor
                            font.pixelSize: textSize
                            text: qsTr(idRepeater.model[index].title) + trans.transString
                            anchors.centerIn: parent
                        }
                    }
                    Loader{
                        id:idLoader
                        sourceComponent: getComponent(idRepeater.model[index].type)
                        Component.onCompleted: {
                            item.setDefault(idRepeater.model[index].defaultValue)
                        }
                    }
                    Connections{
                        target: idLoader.item
                        onSigChanged: function(value){
                            idRepeater.mModel[index].defaultValue.value = value
                            sigChanged()
                        }
                    }
                }
            }
            function setDefault(d){
                idRepeater.mModel = d
            }
        }
    }

    Component{
        id:idObjectComponent
        Column{
            signal sigChanged(var value)
            spacing: flowSpacing
            Repeater{
                property var mModel: []
                id:idRepeater
                model: idRepeater.mModel
                delegate: Row {
                    spacing: flowSpacing
                    Rectangle{
                        width: leftWidth
                        height: colHeight
                        color: tipColor
                        radius: 4
                        clip: true
                        Text {
                            id:idTitle
                            color: tipTxtColor
                            font.pixelSize: textSize
                            text: qsTr(idRepeater.model[index].title) + trans.transString
                            anchors.centerIn: parent
                        }
                    }
                    Loader{
                        id:idLoader
                        sourceComponent: getComponent(idRepeater.model[index].type)
                        Component.onCompleted: {
                            item.setDefault(idRepeater.model[index].defaultValue)
                        }
                    }
                    Connections{
                        target: idLoader.item
                        onSigChanged: function(value){
                            idRepeater.mModel[index].defaultValue.value = value
                            sigChanged(idRepeater.mModel)
                        }
                    }
                }
            }
            function setDefault(d){
                idRepeater.mModel = d.value
            }
        }
    }

    Component{
        id:idStringComponent
        Rectangle{
            signal sigChanged(var value)
            height: colHeight
            width: itemWidth
            border.width: 1
            border.color: "#888"
            color: "#aaa"
            clip: true
            TextInput{
                id:idStringInput
                anchors.fill: parent
                anchors.margins: 2
                selectByMouse: true
                activeFocusOnTab : true
                font.pixelSize: textSize
                validator: RegExpValidator{regExp: /[\x00-\xff]+/g }
                onTextEdited: {
                    sigChanged(String(text))
                }
            }
            function setDefault(value){
                idStringInput.text = String(value.value)
            }
        }
    }

    Component{
        id:idNumberComponent
        Rectangle{
            signal sigChanged(var value)
            height: colHeight
            width: itemWidth
            border.width: 1
            border.color: "#888"
            color: "#aaa"
            clip: true
            TextInput{
                id:idNumberTextInput
                anchors.fill: parent
                anchors.margins: 2
                selectByMouse: true
                activeFocusOnTab : true
                font.pixelSize: textSize
                validator: DoubleValidator{
                }
                onTextEdited: {
                    sigChanged(Number(text))
                }
            }
            function setDefault(value){
                idNumberTextInput.text = String(value.value)
            }
        }
    }

    Component{
        id:idBoolComponent
        Rectangle{
            signal sigChanged(var value)
            height: colHeight
            width: itemWidth
            color: "transparent"
            clip: true
            CheckBox{
                id:idComboBox
                onClicked: sigChanged(checked)
                implicitHeight: colHeight
            }

            function setDefault(value){
                var v = value.value
                idComboBox.checked = v
            }
        }
    }

    Component{
        id:idNumberArrayComponent
        Flow{
            signal sigChanged(var value)
            property var mArray: []
            id:idMainFlow
            width: itemWidth
            spacing: flowSpacing
            Repeater{
                model: mArray.length
                delegate: Flow{
                    Rectangle{
                        width: 50
                        height: 30
                        border.width: 1
                        border.color: "#eee"
                        clip: true
                        TextInput{
                            id:idNumberTextInput
                            anchors.fill: parent
                            anchors.margins: 2
                            clip: true
                            selectByMouse: true
                            activeFocusOnTab : true
                            font.pixelSize: textSize
                            text: String(idMainFlow.mArray[index])
                            validator: DoubleValidator{
                            }
                            onTextEdited: {
                                idMainFlow.mArray[index] = Number(text)
                                sigChanged(idMainFlow.mArray)
                            }
                        }
                    }
                    Text {
                        text: qsTr("-")
                        font.pixelSize: 30
                        color: "red"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                var arr = []
                                for(var i = 0; i < idMainFlow.mArray.length; i++){
                                    if(i !== index){
                                        arr.push(idMainFlow.mArray[i])
                                    }
                                }
                                sigChanged(arr)
                                idMainFlow.mArray = arr
                            }
                            hoverEnabled: true
                            onEntered: cursorShape = Qt.PointingHandCursor
                            onExited: cursorShape = Qt.ArrowCursor
                        }
                    }
                }
            }

            Text {
                font.pixelSize: 30
                color: "lightblue"
                text: qsTr("+")
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var arr = idMainFlow.mArray
                        arr.push(0)
                        idMainFlow.mArray = arr
                        sigChanged(idMainFlow.mArray)
                    }
                    hoverEnabled: true
                    onEntered: cursorShape = Qt.PointingHandCursor
                    onExited: cursorShape = Qt.ArrowCursor
                }
            }
            function setDefault(value){
                idMainFlow.mArray = value.value
            }
        }
    }

    Component{
        id:idStringArrayComponent
        Flow{
            signal sigChanged(var value)
            property var mArray: []
            id:idMainFlow
            width: itemWidth
            spacing: flowSpacing
            Repeater{
                model: mArray.length
                delegate: Flow{
                    spacing: flowSpacing
                    Rectangle{
                        width: itemWidth - 20
                        height: 30
                        border.width: 1
                        border.color: "#eee"
                        clip: true
                        TextInput{
                            id:idNumberTextInput
                            anchors.fill: parent
                            anchors.margins: 2
                            clip: true
                            selectByMouse: true
                            activeFocusOnTab : true
                            font.pixelSize: textSize
                            text: String(idMainFlow.mArray[index])
                            onTextEdited: {
                                idMainFlow.mArray[index] = text
                                sigChanged(idMainFlow.mArray)
                            }
                        }
                    }
                    Text {
                        text: qsTr("-")
                        font.pixelSize: 30
                        color: "red"
                        MouseArea{
                            anchors.fill: parent
                            onClicked: {
                                var arr = []
                                for(var i = 0; i < idMainFlow.mArray.length; i++){
                                    if(i !== index){
                                        arr.push(idMainFlow.mArray[i])
                                    }
                                }
                                sigChanged(arr)
                                idMainFlow.mArray = arr
                            }
                            hoverEnabled: true
                            onEntered: cursorShape = Qt.PointingHandCursor
                            onExited: cursorShape = Qt.ArrowCursor
                        }
                    }
                }
            }

            Text {
                font.pixelSize: 30
                color: "lightblue"
                text: qsTr("+")
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var arr = idMainFlow.mArray
                        arr.push("")
                        idMainFlow.mArray = arr                        
                        sigChanged(idMainFlow.mArray)
                    }
                    hoverEnabled: true
                    onEntered: cursorShape = Qt.PointingHandCursor
                    onExited: cursorShape = Qt.ArrowCursor
                }
            }
//            onMArrayChanged: sigChanged(mArray)
            function setDefault(value){
                idMainFlow.mArray = value.value
            }
        }
    }

    Component{
        id:idObjectArrayComponent
        Rectangle{
            id:idMainRect
            signal sigChanged(var value)
            property var mArray: []
            property var base: []
            width: idMainFlow.implicitWidth ? idMainFlow.implicitWidth : 100
            height:  Math.max(idMainFlow.implicitHeight  + border.width * 2
                              , colHeight)
            border.width: 1
            border.color: colorArray[1]
            color: backgroundColor
            clip: true
            Column{
                id:idMainFlow
                x: parent.border.width
                y: parent.border.width
                spacing: flowSpacing
                Repeater{
                    model: mArray
                    delegate: Rectangle{
                        id:idParentRect
                        width: idParentFlow.implicitWidth + 2
                        height: Math.max(idParentFlow.implicitHeight, colHeight)
                        border.width: 1
                        border.color: colorArray[1]
                        color: backgroundColor
                        clip: true
                        Column{
                            property var mIndex: index
                            property var subArray: idMainRect.mArray[index]
                            id:idParentFlow
                            x : parent.border.width
                            y : parent.border.width
                            spacing: flowSpacing
                            Repeater{
                                model: idParentFlow.subArray
                                delegate: Row{
                                    spacing: flowSpacing
                                    Rectangle{
                                        width: leftWidth
                                        height: colHeight
                                        color: tipColor
                                        radius: 4
                                        clip: true
                                        Text {
                                            id:idTitle
                                            color: tipTxtColor
                                            font.pixelSize: textSize
                                            text: qsTr(String(idParentFlow.subArray[index].title)) + trans.transString
                                            anchors.centerIn: parent
                                        }
                                    }
                                    Loader{
                                        id:idLoader
                                        sourceComponent: getComponent(idParentFlow.subArray[index].type)
                                        Component.onCompleted: {
                                            item.setDefault(idParentFlow.subArray[index].defaultValue)
                                        }
                                    }

                                    Connections{
                                        target: idLoader.item
                                        onSigChanged: function(value){
                                            idMainRect.mArray[idParentFlow.mIndex][index].defaultValue.value = value
                                            idMainRect.sigChanged(idMainRect.mArray)
                                        }
                                    }
                                }
                            }

                            Text {
                                text: qsTr("-")
                                font.pixelSize: 30
                                color: "red"
                                MouseArea{
                                    anchors.fill: parent
                                    onClicked: {
                                        var arr = []
                                        for(var i = 0; i < idMainRect.mArray.length; i++){
                                            if(i !== index){
                                                arr.push(idMainRect.mArray[i])
                                            }
                                        }
                                        idMainRect.sigChanged(arr)
                                        idMainRect.mArray = arr
                                    }
                                    hoverEnabled: true
                                    onEntered: {
                                        idParentRect.border.color = colorArray[0]
                                        cursorShape = Qt.PointingHandCursor
                                    }
                                    onExited: {
                                        idParentRect.border.color = colorArray[1]
                                        cursorShape = Qt.ArrowCursor
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Text {
                font.pixelSize: 30
                color: "green"
                text: qsTr("+")
                anchors.left: parent.left
                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        var arr = idMainRect.mArray
                        let base = []
                        let base_str = JSON.stringify(idMainRect.base)
                        base = JSON.parse(base_str)
                        arr.push(base)
                        idMainRect.sigChanged(arr)
                        idMainRect.mArray = arr
                    }
                    hoverEnabled: true
                    onEntered: {
                        idMainRect.border.color = colorArray[0]
                        cursorShape = Qt.PointingHandCursor
                    }
                    onExited: {
                        idMainRect.border.color = colorArray[1]
                        cursorShape = Qt.ArrowCursor
                    }
                }
            }
            function setDefault(value){
                idMainRect.mArray = value.value
                idMainRect.base = value.base
            }
        }
    }

    function getComponent(type){
        switch(type){
        case "object":
            return idObjectComponent
        case "string":
            return idStringComponent
        case "int" :
        case "double":
            return idNumberComponent
        case "bool":
            return idBoolComponent
        case "number_array":
            return idNumberArrayComponent
        case "string_array":
            return idStringArrayComponent
        case "object_array":
            return idObjectArrayComponent
        default :
            console.log("unknown type",type)
            break;
        }
    }
}


