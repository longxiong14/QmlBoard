import QtQuick 2.13
import QtQuick.Controls 1.4
import "../operators"
GroupBox{
    title: qsTr("translate")
    Row{
        spacing: 20
        CVPoint{
            id:idPoint
            title: qsTr("delta point")
        }

        Button{
            text: qsTr("translate")
            onClicked: {
                gCtrl.translate(idPoint.cvPoint)
            }
        }
    }
}
