// Main.qml
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
import QtCore
import cardmix

ApplicationWindow {
    id: window
    width: 500
    height: 1000
    visible: true
    title: qsTr("Hello Layouts")

    required property VisualCardsModel blocksListModel

    header: TextField {
        id: filterText

        placeholderText: qsTr("Enter a block type name")
        text: "" // window.filterModel.lastName
        font.pointSize: 18
        onEditingFinished: {
            focus = false
            // window.filterModel.lastName = text
        }

        Keys.onEscapePressed: {
            focus = false
        }
    }

    // The main view (ensure it has objectName/id so buttons can access the model)
    CardsListView {
        id: blocksListView
        blocksModel: window.blocksListModel
        viewInteraction: true
        title: qsTr("Blocks List")
    }
}
