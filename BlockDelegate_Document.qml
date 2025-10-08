pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import blockeditor

BlockDelegateLayout {
    id: root
    required property var blockContent
    required property int index            // ListView's row index

    listView: ListView.view
    itemIndex: index
    imageIndex: "red_danger.svg"

    Column {
        id: documentColumn
        spacing: 6
        Label { text: "Document"; font.bold: true }
        Label { text: "Document Number: " + (blockContent?.documentNumber ?? "") }
        Label { text: "Material: " + (blockContent?.material ?? "") }
        Label { text: "Mesh density: " + (blockContent?.meshDensity ?? "") }
    }
}
