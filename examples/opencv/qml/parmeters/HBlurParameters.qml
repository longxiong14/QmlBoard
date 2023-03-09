import QtQuick 2.13
import QtQuick.Controls 1.4
import "../operators"
GroupBox{
    title: qsTr("blur")
    Row{
        spacing: 20
        CVSize{
            id:idSize
        }

        CVPoint{
        }

        Button{
            text: qsTr("blur")
            onClicked: {
                gCtrl.blur(idSize.cvSize)
            }
        }
    }
}
