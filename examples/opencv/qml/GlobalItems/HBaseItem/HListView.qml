import QtQuick 2.9
import QtQuick.Controls 1.4

ScrollView{
    property var mModel: []
    property int itemWidth: 100
    property int itemHeight: 30
    id:idRoot
    ListView{
        id:idMainListView
        width: idRoot.width
        model: mModel
        spacing: 5
        currentIndex: -1
        delegate: HRectangleTextButton{
            txt: qsTr(mModel[index].text) + trans.transString
            width: itemWidth
            height: itemHeight
            enabled: Boolean(gUserModel.userPermissionArray.includes(mModel[index].text)) || gUserModel.superUser
            radius: 0
            onSigClicked: {
                idMainListView.currentIndex = index
                mModel[index].func()
            }
            colorArray: idMainListView.currentIndex === index ?
                            ["#0065E7","#92BD6C","#B0C8F9"] :
                            ["transparent","#92BD6C","#B0C8F9"]
        }
        onActiveFocusChanged: {
        }
    }
    function setCurrentIndex(index){
        idMainListView.currentIndex = index
    }
}
