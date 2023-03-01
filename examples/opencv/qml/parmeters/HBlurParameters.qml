import QtQuick 2.0
import QtQuick.Controls 1.4
Flow{
    Text {
        text: qsTr("blur")
    }

    Button{
        text: qsTr("blur")
        onClicked: gCtrl.blur({})
    }
}
