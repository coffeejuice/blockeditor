pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import blockeditor

BlockDelegateLayout {
    id: root
    // required property var blockContent
    // required property var listModel
    required property var a
    required property var b
    required property var c
    required property var d
    required property int index            // ListView's row index

    listView: ListView.view
    itemIndex: index
    imageIndex: "red_danger.svg"

    Column {
        id: documentColumn
        spacing: 6
        Label { text: "Document"; font.bold: true }
        Row {
            spacing: 8
            focus: true
            focusPolicy: Qt.StrongFocus
            onActiveFocusChanged: if (activeFocus) root.listView.currentIndex = root.itemIndex
            Label { text: "Document Number: "; width: 110; horizontalAlignment: Text.AlignRight }
            TextField { text: a; placeholderText: "Enter document number"; onEditingFinished: listView.model.setField(index,"a",text) }
        }
        Row {
            spacing: 8
            focus: true
            focusPolicy: Qt.StrongFocus
            onActiveFocusChanged: if (activeFocus) root.listView.currentIndex = root.itemIndex
            Label { text: "Material: "; width: 110; horizontalAlignment: Text.AlignRight }
            TextField { text: b; placeholderText: "Enter document number"; onEditingFinished: listView.model.setField(index,"b",text) }
        }

        Row {
            spacing: 8
            focus: true
            focusPolicy: Qt.StrongFocus
            onActiveFocusChanged: if (activeFocus) root.listView.currentIndex = root.itemIndex
            Label { text: "Mesh elements along width: "; width: 110; horizontalAlignment: Text.AlignRight }
            TextField { text: c; placeholderText: "Enter document number"; onEditingFinished: listView.model.setField(index,"c",text) }
        }

        Row {
            spacing: 8
            focus: true
            focusPolicy: Qt.StrongFocus
            onActiveFocusChanged: if (activeFocus) root.listView.currentIndex = root.itemIndex
            Label { text: "Weight [kg]: "; width: 110; horizontalAlignment: Text.AlignRight }
            TextField { text: d; placeholderText: "Enter document number"; onEditingFinished: listModel.setField(index,"d",text) }
        }
    }
}
