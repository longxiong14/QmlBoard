import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.13
import QtQuick.Window 2.13
import "../HBaseItem"

Flow{
    property int colHeight: 30
    property int leftWidth: 100
    property int minWidth: 0
    property var mModel: []
    property int textSize: 18
    property var lastestValue: ""
    property var tipColor: "transparent" //"#4963ff"
    property var tipTxtColor: "white"
    signal sigValueChanged(var title, var value, var type)

    property var isInit : true
    spacing: 5
    Repeater{
        id:idRepeater
        model: mModel
        delegate: Flow{
            id: idRowFlow
            width: minWidth > 0 ? minWidth :
                                  parent.width < (leftWidth + 200) ? (leftWidth + 200 ) : parent.width

            Flow {
                width: idRowFlow.width
                Rectangle{
                    width: leftWidth
                    height: colHeight
                    color: tipColor
                    radius: 4
                    Text {
                        id:idTitle
                        color: tipTxtColor
                        font.pixelSize: textSize
                        text: qsTr(mModel[index].title) + trans.transString
                        anchors.centerIn: parent
                    }
                }
                Loader{
                    id:idLoader
                    width: parent.width - leftWidth - parent.spacing * 2
                    sourceComponent: getComponent(mModel[index].type)
                    Component.onCompleted: {
                        item.setDefault(mModel[index].defaultValue)
                    }
                }
                Connections{
                    target: idLoader.item
                    onSigChanged: function(value){
                        if(!isInit){
                            lastestValue = {
                                title : mModel[index].title,
                                value : value
                            }
                            sigValueChanged(mModel[index].title, value, mModel[index].type)
                        }
                    }
                }
            }
        }
    }

    Component{
        id:idStringComponent
        HTextArea{
            signal sigChanged(var value)
            id:idInputArea
            font.pixelSize: textSize
            width: parent.width
            height: colHeight * 2
            onTextChanged: {
                sigChanged(String(text))
            }
            function setDefault(value){
                idInputArea.text = value.value
            }
        }
    }

    Component{
        id:idNumberComponent
        Rectangle{
            signal sigChanged(var value)
            height: colHeight
            width: parent.width
            border.width: 1
            border.color: "#888"
            color: "#aaa"
            TextInput{
                id:idNumberTextInput
                anchors.fill: parent
                anchors.margins: 2
                selectByMouse: true
                activeFocusOnTab : true
                font.pixelSize: textSize
                validator: DoubleValidator{
                }
                onTextChanged: {
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
            width: parent.width
            color: "transparent"

            CheckBox{
                id:idComboBox
                onClicked: sigChanged(checked)
                implicitHeight: colHeight
            }

            function setDefault(value){
                var v = value.value
                idComboBox.checked = v
            }

            //            HComboBox{
            //                id:idComboBox
            //                anchors.fill: parent
            //                anchors.margins: 2
            //                model:["false", "true"]
            //                onActivated : {
            //                    if(1 === currentIndex)
            //                        sigChanged(true)
            //                    else{
            //                        sigChanged(false)
            //                    }
            //                }
            //            }
            //            function setDefault(value){
            //                var v = value.value
            //                if(v === false || v === "false"){
            //                    idComboBox.currentIndex = 0
            //                }else{
            //                    idComboBox.currentIndex = 1
            //                }
            //            }
        }
    }

    Component{
        id:idRectangleComponent
        Flow{
            property var txt: ["x", "y", "width", "height"]
            property var values: [0,0,0,0]
            signal sigChanged(var value)
            id:idFlow
            width: parent.width
            spacing: 5
            Repeater{
                model: idFlow.txt.length
                delegate: Rectangle{
                    width: 150
                    height: 26
                    Row{
                        height: parent.height
                        spacing: 2
                        Rectangle{
                            width: 60
                            height: parent.height
                            Text {
                                anchors.centerIn: parent
                                text: idFlow.txt[index]
                                font.pixelSize: textSize
                                horizontalAlignment: Text.AlignRight
                            }
                        }

                        Rectangle{
                            width: 100
                            height: parent.height
                            border.width: 2
                            border.color: "#eee"
                            TextInput{
                                anchors.fill: parent
                                anchors.margins: 2
                                selectByMouse: true
                                text: idFlow.values[index]
                                font.pixelSize: textSize
                                activeFocusOnTab : true
                                validator: DoubleValidator{
                                }
                                onTextChanged: {
                                    idFlow.values[index] = Number(text)
                                    sigChanged(idFlow.values)
                                }
                            }
                        }
                    }
                }
            }
            function setDefault(value){
                values = value.value
            }
        }
    }

    Component{
        id:idSelectComponent
        Flow{
            width: parent.width
            height: colHeight
            signal sigChanged(var value)
            property var mModel: []
            HComboBox{
                id:idSelectCombobox
                width: parent.width - 60
                height: parent.height
                currentIndex: -1
                font.pixelSize: textSize
                editable: true
                model: mModel
                onActivated : {
                    sigChanged(mModel[currentIndex])
                }
            }
            HButton{
                text: "搜索"
                width: 60
                height: colHeight
                onClicked: {
                    idWindow.showFind(mModel)
                }
            }

            function setDefault(value){
                mModel = value.list
                idSelectCombobox.currentIndex = -1
                for(var i = 0; i < mModel.length; i++){
                    if(i === value.currentIndex || value.value === mModel[i]){
                        idSelectCombobox.currentIndex = i
                    }
                }
            }

            Window{
                property var mModel: []
                property var lists: []
                property var indexs: []
                property var leftTipWidth: 100
                id:idWindow
                title: "搜索"
                visible: false
                width: 500
                height: 300

                Flow{
                    width: parent.width
                    spacing: 5
                    Rectangle{
                        width: idWindow.leftTipWidth
                        height: colHeight
                        radius: 10
                        color:  Qt.rgba(146/255,189/255,108/255)
                        Text {
                            text: "搜索"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: textSize
                        }
                    }
                    HTextArea{
                        width: parent.width - idWindow.leftTipWidth - parent.spacing
                        onTextChanged: {
                            if("" === text )return
                            let l = [],idx = []
                            for(var i = 0; i < idWindow.mModel.length; i++){
                                let txt = idWindow.mModel[i]
                                if(-1!==txt.indexOf(text)){
                                    l.push(txt)
                                    idx.push(i)
                                }
                            }
                            if(0 === l.lenght) return
                            idWindow.indexs = idx
                            idFindCombo.showModel(l)
                            idWindow.lists = l
                        }
                    }
                    Rectangle{
                        width: idWindow.leftTipWidth
                        height: colHeight
                        radius: 10
                        color:  Qt.rgba(146/255,189/255,108/255)
                        Text {
                            text: "搜索结果"
                            anchors.centerIn: parent
                            color: "white"
                            font.pixelSize: textSize
                        }
                    }
                    HComboBox{
                        property bool init: false
                        id:idFindCombo
                        width: parent.width - idWindow.leftTipWidth - parent.spacing
                        onActivated: {
                            if(-1 !== currentIndex && init)
                                idSelectCombobox.currentIndex = idWindow.indexs[currentIndex]
                        }
                        onModelChanged: {
                            currentIndex = -1
                            init = false
                        }
                        function showModel(m){
                            init = false
                            currentIndex = -1
                            model = m
                            init = true
                        }
                    }
                }

                function showFind(m){
                    mModel = m
                    lists = []
                    indexs = []
                    show()
                }
            }
        }
    }

    Component{
        id:idMultiselect
        Flow{
            id:idMultiSelectFlow
            property var mModel: []
            property var mSelected: []
            signal sigChanged(var value)
            width: parent.width
            spacing: 5
            Repeater{
                model: mModel.length
                delegate: CheckBox{
                    height: 20
                    text: idMultiSelectFlow.mModel[index]
                    checked: idMultiSelectFlow.mSelected[index]
                    onCheckedChanged: {
                        mSelected[index] = checked
                        idMultiSelectFlow.sigOutValue()
                    }
                }
            }
            function setDefault(value){
                var md = value.list
                var df = value.value
                for(var i = 0; i < md.length; i++){
                    var lable = md[i]
                    if(-1 === df.indexOf(lable)){
                        mSelected.push(false)
                    }else{
                        mSelected.push(true)
                    }
                }
                mModel = md
            }
            function sigOutValue(){
                if(mModel.length === mSelected.length){
                    var list = []
                    for(var i = 0; i < mSelected.length; i++){
                        if(true === mSelected[i]){
                            list.push(mModel[i])
                        }
                    }
                    sigChanged(list)
                }
            }
        }
    }

    Component{
        id:idFileSelectComponent
        Rectangle{
            signal sigChanged(var value)
            property var model: 0
            width: parent.width
            height: idFlow.implicitHeight
            border.color: "#eee"
            border.width: 1
            Flow{
                id:idFlow
                width: parent.width
                HTextArea {
                    id: idFileShowText
                    width: parent.width - 60
                    font.pixelSize: textSize
                    onTextChanged: {
                        sigChanged(text)
                    }
                }
                HButton{
                    width: 60
                    implicitHeight: colHeight
                    text: "选择"
                    onClicked: {
                        switch (model){
                        case 0:
                            idFileDialog.openFile(["所有文件 (*.*)"])
                            break;
                        case 1:
                            idFileDialog.saveFile()
                            break;
                        case 2:
                            idFileDialog.openFolder()
                            break;
                        default:
                            idFileDialog.openFile(["所有文件 (*.*)"])
                            break;
                        }
                    }
                }
            }

            HFileDialog{
                id:idFileDialog
                onSigOpenFile: {
                    idFileShowText.text = file
                }
                onSigSaveFile: {
                    idFileShowText.text = file
                }
                onSigOpenFolder: {
                    idFileShowText.text = path
                }
            }
            function setDefault(value){
                model = value.model
                idFileShowText.text = String(value.value)
            }
        }
    }

    Component{
        id:idRadioSelectComponent
        Flow{
            id:idRadiosFlow
            property var mModel: ["1", "2", "3"]
            property int mCurrentIndex: -1
            property var mSelected: []
            signal sigChanged(var value)
            width: parent.width
            spacing: 5
            ButtonGroup{
                buttons:idRepeater.children
            }
            Repeater{
                id:idRepeater
                model: mModel.length
                delegate: RadioButton{
                    text: mModel[index]
                    checked: idRadiosFlow.mCurrentIndex === index
                    onClicked: {
                        idRadiosFlow.mCurrentIndex = index
                    }
                }
            }

            function setDefault(value){
                mModel = value.list
                mCurrentIndex = -1
                for(var i = 0; i < mModel.length; i++){
                    if(i === value.value || mModel[i] === value.value){
                        mCurrentIndex = i
                    }
                }
            }

            onMCurrentIndexChanged: {
                sigChanged(mModel[mCurrentIndex])
            }
        }
    }

    Component{
        id:idSliderComponent
        Slider {
            signal sigChanged(var value)
            id:idSlider
            from: 0
            value: 0
            to: 255
            width: parent.width
            stepSize: 1.0
            onValueChanged: {
                sigChanged(value)
            }
            onHoveredChanged: {
                idToolTip.visible = hovered
            }
            ToolTip {
                id:idToolTip
                visible: false
                text: idSlider.value
                delay: 100
            }
            function setDefault(value){
                idSlider.value = Number(value.value)
                idSlider.stepSize = Number(value.step)
            }
        }
    }


    Component{
        id:idJsonComponent
        Flow{
            signal sigChanged(var value)
            id:idFlow
            width: parent.width
            spacing: 5
            Column{
                width: parent.width
                Text{
                    id:idTipWarning
                    text: "json parse error"
                    height: 30
                    visible: false
                    font.pixelSize: textSize
                    color: "red"
                }
                HTextArea{
                    id:idInputArea
                    font.pixelSize: textSize
                    width: idFlow.width
                    onTextChanged: {
                        try{
                            var object = JSON.parse(text)
                            idTipWarning.visible = false
                            sigChanged(object)
                        }catch(e){
                            idTipWarning.visible = true
                        }
                    }
                }
            }
            function setDefault(value){
                try{
                    if(undefined === value.parse || true === value.parse)
                        idInputArea.text = JSON.stringify(value.value, null, 2)
                    else
                        idInputArea.text = JSON.stringify(value.value)
                }catch(e){
                    idInputArea.text = "json parse error"
                }
            }
        }
    }

    Component{
        id:idPasswordComponent
        Rectangle{
            signal sigChanged(var value)
            width: parent.width
            height: colHeight
            border.width: 1
            border.color: "#aaa"
            TextInput{
                id:idInputArea
                font.pixelSize: textSize
                //                width: parent.width
                echoMode: TextInput.Password
                anchors.fill: parent
                anchors.margins: 2
                onTextChanged: {
                    sigChanged(String(text))
                }
            }

            function setDefault(value){
                idInputArea.text = value.value
            }
        }
    }


    Component{
        id:idNumberArrayComponent
        Flow{
            signal sigChanged(var value)
            property var mArray: []
            id:idMainFlow
            width: parent.width
            spacing: 10
            Repeater{
                model: mArray.length
                delegate: Flow{
                    Rectangle{
                        width: 50
                        height: 30
                        border.width: 1
                        border.color: "#eee"
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
                            onTextChanged: {
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
            width: parent.width
            spacing: 10
            Repeater{
                model: mArray.length
                delegate: Flow{
                    Rectangle{
                        width: 300
                        height: 30
                        border.width: 1
                        border.color: "#eee"
                        TextInput{
                            id:idNumberTextInput
                            anchors.fill: parent
                            anchors.margins: 2
                            clip: true
                            selectByMouse: true
                            activeFocusOnTab : true
                            font.pixelSize: textSize
                            text: String(idMainFlow.mArray[index])
                            onTextChanged: {
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
        id:idInRangeSlider
        Rectangle{
            id:idRectangle
            property var out: ({})
            property var precision: 2
            signal sigChanged(var value)
            width: parent.width
            height: idMainFlow.implicitHeight
            border.width: 1
            border.color: "#aaa"
            Flow{
                id:idMainFlow
                width: parent.width
                RangeSlider{
                    id:idRangeSlider
                    width: parent.width
                    snapMode: RangeSlider.SnapAlways
                    first.onMoved: {
                        outValue()
                    }
                    second.onMoved: {
                        outValue()
                    }
                }
                Rectangle{
                    width: parent.width
                    height: Math.max(idFirstText.implicitHeight, idSecontText.implicitHeight)
                    color: "transparent"
                    Text {
                        id:idFirstText
                        font.pixelSize: textSize
                        text: Math.round(idRangeSlider.first.value * precision) / precision
                        anchors.left: parent.left
                    }
                    Text {
                        id:idSecontText
                        font.pixelSize: textSize
                        text:  Math.round(idRangeSlider.second.value * precision) / precision
                        anchors.right: parent.right
                    }
                }
            }

            function outValue(){
                let o = {}
                o.first = Math.round(idRangeSlider.first.value * precision) / precision
                o.second = Math.round(idRangeSlider.second.value * precision) / precision
                console.log(JSON.stringify(o))
                sigChanged(o)
            }

            function setDefault(v){
                let value = v.value
                idRangeSlider.from = value.from
                idRangeSlider.to = value.to
                idRangeSlider.second.value = value.second
                idRangeSlider.first.value = value.first
                idRangeSlider.stepSize = value.step_size
                precision = Math.pow(10, value.precision)
            }
        }
    }


    Component{
        id:idInRangeInput
        Flow{
            id:idRangeFlow
            signal sigChanged(var value)
            property var out: ({})
            width: parent.width
            spacing: 5


            Rectangle{
                width: 30
                height: colHeight
                border.width: 1
                border.color: "#888"
                color: "#aaa"
                TextInput{
                    id:idLeftTextInput
                    anchors.fill: parent
                    anchors.margins: 2
                    selectByMouse: true
                    activeFocusOnTab : true
                    font.pixelSize: textSize
                    validator: DoubleValidator{
                    }
                    onTextChanged: {
                        changeValue()
                    }
                }
            }

            Rectangle{
                width: 30
                height: colHeight
                border.width: 1
                border.color: "#888"
                color: "#aaa"
                TextInput{
                    id:idRightTextInput
                    anchors.fill: parent
                    anchors.margins: 2
                    selectByMouse: true
                    activeFocusOnTab : true
                    font.pixelSize: textSize
                    validator: DoubleValidator{
                    }
                    onTextChanged: {
                        changeValue()
                    }
                }
            }


            Text {
                id: idUnit
                font.pixelSize: textSize
                height: colHeight
            }

            function changeValue(){
                var o = {
                    min:Number(idLeftTextInput.text),
                    max:Number(idRightTextInput.text)
                }
                sigChanged(o)
            }

            function setDefault(value){
                let v = value.value
                idLeftTextInput.text = String(v.min)
                idRightTextInput.text = String(v.max)
                idUnit.text = String(v.unit)
            }
        }
    }


    Component{
        id:idObjectArrayComponent
        Flow{
            signal sigChanged(var value)
            property var mArray: []
            property var base: []
            id:idMainFlow
            width: parent.width
            spacing: 10
            Repeater{
                model: mArray
                delegate: Flow{
                    property var mIndex: index
                    property var subArray: idMainFlow.mArray[index]
                    id:idParentFlow
                    width: parent.width
                    Repeater{
                        model: idParentFlow.subArray
                        delegate: Flow{
                            width: parent.width
                            HRectangle{
                                width: leftWidth
                                height: colHeight
                                color: tipColor
                                radius: 4
                                HText {
                                    id:idTitle
                                    color: tipTxtColor
                                    font.pixelSize: textSize
                                    text: qsTr(String(idParentFlow.subArray[index].title)) + trans.transString
                                    anchors.centerIn: parent
                                }
                            }
                            Loader{
                                id:idLoader
                                width: parent.width - leftWidth - parent.spacing * 2
                                sourceComponent: getComponent(idParentFlow.subArray[index].type)
                                Component.onCompleted: {
                                    item.setDefault(idParentFlow.subArray[index].defaultValue)
                                }
                            }

                            Connections{
                                target: idLoader.item
                                onSigChanged: function(value){
                                    if(!isInit){
                                        idMainFlow.mArray[idParentFlow.mIndex][index].defaultValue.value = value
                                        let o = []
                                        for(let i = 0; i < idMainFlow.mArray.length; i ++){
                                            let sub = idMainFlow.mArray[i]
                                            let s = {}
                                            for(let j = 0; j < sub.length; j++){
                                                s[sub[j].title] = sub[j].defaultValue.value
                                            }
                                            o.push(s)
                                        }
                                        idMainFlow.sigChanged(o)
                                    }
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
                                for(var i = 0; i < idMainFlow.mArray.length; i++){
                                    if(i !== index){
                                        arr.push(idMainFlow.mArray[i])
                                    }
                                }
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
                        let base = []
                        let base_str = JSON.stringify(idMainFlow.base)
                        base = JSON.parse(base_str)
                        arr.push(base)
                        idMainFlow.mArray = arr
                    }
                    hoverEnabled: true
                    onEntered: cursorShape = Qt.PointingHandCursor
                    onExited: cursorShape = Qt.ArrowCursor
                }
            }

            function setDefault(value){
                idMainFlow.mArray = value.value
                idMainFlow.base = value.base
            }
        }
    }

    function getComponent(type){
        switch(type){
        case "string":
            return idStringComponent
        case "int" :
        case "double":
            return idNumberComponent
        case "bool":
            return idBoolComponent
        case "rectangle":
            return idRectangleComponent
        case "select":
            return idSelectComponent
        case "multiselect":
            return idMultiselect
        case "file":
            return idFileSelectComponent
        case "radios":
            return idRadioSelectComponent
        case "json":
            return idJsonComponent
        case "slider":
            return idSliderComponent
        case "password":
            return idPasswordComponent
        case "number_array":
            return idNumberArrayComponent
        case "string_array":
            return idStringArrayComponent
        case "range_slider":
            return idInRangeSlider
        case "range_input":
            return idInRangeInput
        case "object_array":
            return idObjectArrayComponent
        default :
            return idStringComponent
        }
    }
    onMModelChanged: {
        if(!mModel)return
        isInit = true
        idRepeater.model = 0
        idRepeater.model = mModel.length
        isInit = false
    }
}


