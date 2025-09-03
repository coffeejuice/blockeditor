// SelectionController.qml (Singleton via qmldir or just a plain Item you instantiate)
import QtQuick 2.15

QtObject {
    id: sel
    // anchor points
    property int startIndex: -1
    property int startPos: 0
    property int endIndex: -1
    property int endPos: 0
    signal selectionChanged()

    function clear() {
        startIndex = endIndex = -1; startPos = endPos = 0; selectionChanged();
    }
    function setRange(sIndex, sPos, eIndex, ePos) {
        startIndex = sIndex; startPos = sPos;
        endIndex = eIndex;   endPos = ePos;
        selectionChanged();
    }
    // normalize (start <= end)
    function norm() {
        if (startIndex > endIndex || (startIndex === endIndex && startPos > endPos)) {
            const si = startIndex, sp = startPos;
            startIndex = endIndex; startPos = endPos;
            endIndex = si; endPos = sp;
        }
    }
}
