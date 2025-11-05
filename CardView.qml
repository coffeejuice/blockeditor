// CardView.qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    anchors.fill: parent
    required property CardModel cardViewModel

    Column {
        id: leftPannel
        Layout.alignment: Qt.AlignTop
        width: implicitWidth
        Text { text: qsTr("Manage cards:"); font.bold: true}
        Button { text: qsTr("Append Card"); onClicked: lv.model++}
        Button { text: qsTr("Remove Card"); onClicked: lv.model--; enabled: lv.currentIndex >= 0 }
        // Button { text: qsTr("Draw"); onClicked: lv.model.appendCard("draw")}

    }
    ListView {
        id: lv
        anchors {
            left: leftPannel.right
            right: parent.right
        }
        height: parent.height
        spacing: 6
        focus: true
        highlight: Rectangle {color: Qt.rgba(0,0,0,0.06)}
        model: cardViewModel
        delegate: DelegateChooser {
            role: "cardType"
            DelegateChoice { roleValue: root.cardViewModel.IMAGE;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.GRAPHIC;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.PROCESS;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.HEAT;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.FORMING;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.UPSET;    delegate: Draw {} }
            DelegateChoice { roleValue: root.cardViewModel.DRAW;    delegate: Draw {} }
        }
    }
}
