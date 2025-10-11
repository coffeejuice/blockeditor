pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
// import QtQml.Models

import blockeditor

ApplicationWindow {
    id: window
    width: 500
    height: 1000
    visible: true
    title: qsTr("Hello Layouts")

    required property BlocksListModel blocksListModel

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

    Row {
        anchors.fill: parent

        BlocksListView {
            id: blocksListView
            blocksModel: window.blocksListModel
            viewInteraction: filterText.focus ? false : true
            title: qsTr("Blocks List")
        }
    }
}
