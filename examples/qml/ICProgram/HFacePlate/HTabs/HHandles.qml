import QtQuick 2.0
import "../../HGlobalItems"
import QtQuick.Controls 1.4
Tab {
    property var flowSpacing: 5
    title: "工具"

    HRectangle{
        Flow{
            width: parent.width
            spacing: flowSpacing
            HRectangle{
                width: parent.width
                height: 50
                Flow{
                    anchors.fill: parent
                    anchors.margins: 2
                    spacing: flowSpacing
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/arrow.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("arrow")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/rect.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("rect")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/line.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("line")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/fill_poly.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("fill poly")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/poly.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("poly")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/fill_rectangle.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("fill rect")
                        }
                    }
                    HImageButton{
                        width: parent.height
                        height: parent.height
                        mSource:"qrc:/qml/ICProgram/HIcons/HTools/close.png"
                        backColor: icon === mSource ? "red" : "transparent"
                        onSigClick: {
                            icon = mSource
                            setHandle("none")
                        }
                    }
                }
            }

            GroupBox{
                title: "handle"
                width: parent.width
                Flow{
                    spacing: flowSpacing
                    width: parent.width
                    Repeater{
                        id:idRepeater
                        model: items
                        delegate: Flow{
                            spacing: 5
                            Flow{
                                height: gFontSize
                                spacing: 5
                                width: idText.implicitWidth + idInput.width + spacing
                                HText{
                                    id:idText
                                    height: gFontSize
                                    text: (items[index].name ? items[index].name : items[index].key) + " :"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                HTextInput{
                                    id:idInput
                                    width: 50
                                    height: gFontSize
                                    txt: String(items[index].value)
                                    onSigTextChanged: {
                                        if("" !== txt)
                                            idUIControl.setBoardHandleParam(idMainBoard.boardName, mHandle, items[index].name.key, Number(txt))
                                    }
                                }
                            }
                        }
                    }
                }
            }

            GroupBox{
                title: "shape"
                width: parent.width
                Flow{
                    spacing: flowSpacing
                    width: parent.width
                    Repeater{
                        id:idShapeRepeater
                        model: idMainBoard.items
                        delegate: Flow{
                            spacing: 5
                            Flow{
                                height: gFontSize
                                spacing: 5
                                width: idText2.implicitWidth + idInput2.width + spacing
                                HText{
                                    id:idText2
                                    height: gFontSize
                                    text: (idMainBoard.items[index].name ?
                                               idMainBoard.items[index].name :
                                               idMainBoard.items[index].key) + " :"
                                    horizontalAlignment: Text.AlignHCenter
                                    verticalAlignment: Text.AlignVCenter
                                }
                                HTextInput{
                                    id:idInput2
                                    width: 50
                                    height: gFontSize
                                    txt: String(idMainBoard.items[index].value)
                                    onSigTextChanged: {
                                        idUIControl.setBoardNodeParam(idMainBoard.boardName, idMainBoard.items[index].key, Number(txt))
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    function setHandle(handle){
        mHandle = handle
        idUIControl.setBoardHandle(idMainBoard.boardName, handle)
        params = idUIControl.getBoardHandleParam(idMainBoard.boardName, handle)
    }

    property var icon: ""
    property var params: ({})
    property var items: []
    property var mHandle: ""
    onParamsChanged: {
        items = idUIControl.paramToUIItems(params)
    }
}
