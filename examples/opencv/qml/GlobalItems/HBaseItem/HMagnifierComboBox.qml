import QtQuick 2.0
import QtQuick.Controls 1.4
Flow {
    property var list: []
    property int curIndex: -1
    property string curSel: ""

    property int comboWidth:  width - implicitHeight - spacing
    HComboBox{
        id:idCombobox
        model: list
        width: comboWidth
        height: parent.height
        implicitHeight:  parent.height
        currentIndex: curIndex
        onActivated:  {
            curSel = list[currentIndex]
            curIndex = currentIndex
        }
    }

    HImageButton{
        width: 20
        height: width
        mSource: "file:./configs/HIcons/HTools/magnifier.png"
        onSigClick: idFind.show()
    }

    HWindow{
        property var models: []
        id:idFind
        title: qsTr("find") + trans.transString
        width: 300
        height: 200
        color: gModel.backGround
        HFlow{
            width: parent.width
            HText{
                id:idFindText
                text: qsTr("find") + trans.transString
            }
            HTextInput{
                width: parent.width - parent.spacing - idFindText.implicitWidth
                height: 20
                color: "white"
                onSigTextChanged: {
                    let list = idCombobox.model
                    let results = []
                    for(let i = 0; i < list.length; i++){
                        let item = list[i]
                        if(-1 !== item.indexOf(txt)){
                            let r = {
                                txt: item,
                                index: i
                            }
                            results.push(r)
                        }
                    }
                    idFind.models = results
                }
            }

            HFlow{
                width: parent.width
                Repeater{
                    id:idRepeater
                    model: idFind.models
                    delegate: HCheckBox{
                        exclusiveGroup: idEx
                        text: idFind.models[index].txt
                        onCheckedChanged: if(checked) {
                                              let item = idFind.models[index]
                                              idCombobox.currentIndex = item.index
                                              curIndex = item.index
                                              curSel = item.txt
                                          }
                    }
                }
            }

            ExclusiveGroup{
                id:idEx
            }
        }
    }
}
