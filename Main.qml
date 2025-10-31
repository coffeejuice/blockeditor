import QtQuick
import QtQuick.Controls

Window {
    id: root
    width: 1000
    height: 800
    visible: true
    title: qsTr("3-level model ListView->Flow->TextFields")

    required property CardsModel cardsModel

    CardsView {
        anchors.fill: parent
        model: root.cardsModel
    }
}
