// Main.qml
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic
import QtCore
// import Qt.labs.platform as Labs
import blockeditor

ApplicationWindow {
    id: window
    width: 500
    height: 1000
    visible: true
    title: qsTr("Hello Layouts")

    required property BlocksListModel blocksListModel

    // Plain string path (not URL), with forward slash
    // readonly property string documentsPath: Labs.StandardPaths.writableLocation(Labs.StandardPaths.DocumentsLocation)
    // readonly property string jsonPath: documentsPath + "/test.json"

    // header: Row {
    //     id: fileButtons
    //     spacing: 8
    //     anchors {
    //         right: parent.right
    //         rightMargin: 12
    //         top: parent.top
    //         topMargin: 12
    //     }
    //
    //     Button {
    //         text: "Save JSON"
    //         onClicked: {
    //             // console.log("Saving to:", jsonPath)
    //             window.blocksListModel.saveToFile()
    //         }
    //     }
    //
    //     Button {
    //         text: "Load JSON"
    //         onClicked: {
    //             // console.log("Loading from:", jsonPath)
    //             window.blocksListModel.loadFromFile()
    //         }
    //     }
    // }
    // }

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
    BlocksListView {
        id: blocksListView
        blocksModel: window.blocksListModel
        viewInteraction: true
        title: qsTr("Blocks List")
    }
}
