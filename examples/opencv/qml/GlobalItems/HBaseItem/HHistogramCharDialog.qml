import QtQuick 2.0
import hHistogramChart 1.0
import QtQuick.Window 2.13

Window {
    property var histogram: []
    property var out: ({})
    signal sigValueChanged()
    width: 300
    height: 300

    HFlow{
        width: parent.width
        Repeater{
            model: histogram
            delegate: HHistogramChart{
                left: histogram[index].left
                right: histogram[index].right
                array: histogram[index].array
                color: histogram[index].color
                width: parent.width
                height: 100
                onValueChanged: {
                    out[color].left = left
                    out[color].right = right
                    sigValueChanged()
                }
            }
        }
    }
    onHistogramChanged: {
        let o = {}
        for(var i = 0; i < histogram.length; i++){
            let value = {
                left: histogram[i].left,
                right: histogram[i].right
            }
            o[histogram[i].color] = value
        }
        out = o
        if(!visible) show()
        console.log("threshold out changed : ",JSON.stringify(out))
    }
}
