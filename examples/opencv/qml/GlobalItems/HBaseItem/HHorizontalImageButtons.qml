import QtQuick 2.0

HFlow{
    property var list: []
    property var curIndex: -1
    Repeater{
        model: list
        delegate: HImageTextButton{
            border.width: 0
            border.color: "black"
            colorArray: index === curIndex ?
                            ["#0065E7","#91C9F7","#00a2e8"] :
                            ["transparent","#91C9F7","#00a2e8"]
            mSource: list[index].source
            text: qsTr( list[index].text) + trans.transString
            iconSize: gModel.topIconSize
            width: 100
            height: 100
            onSigClick: {
                list[index].func()
            }
        }
    }
}
