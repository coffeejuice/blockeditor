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
    imageIndex: "image4.jpg"

    Column {
        id: upsetColumn
        spacing: 6
        Label { text: "Upset"; font.bold: true }
        Label { text: String(blockContent?.operations ?? "") }
    }
}
