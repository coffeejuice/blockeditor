pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
import QtQml.Models

import blockeditor

ApplicationWindow {
    id: window
    width: 250
    height: 1100
    visible: true
    title: qsTr("Hello Layouts")

    required property BlocksListModel blocksListModel

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
