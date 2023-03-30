import QtQuick 2.0

HMagnifierComboBox {
    signal sigFlush()

    comboWidth: width - implicitHeight - 2*spacing - 20
    HImageButton{
        width: 20
        height: width
        mSource: "file:./configs/HIcons/HTools/flush.png"
        onSigClick: sigFlush()
    }
}
