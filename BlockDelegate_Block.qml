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
    imageIndex: "blocktype_unknown_yellow_inkspace.svg"

    Column {
        id: blockColumn
        spacing: 6
        Label { text: "Begin Block"; font.bold: true }
        Label { text: "Press: " + (blockContent?.press ?? "") }
        Label { text: "Dies: " + (blockContent?.dies ?? "") }
        Row {
            spacing: 12
            Label { text: "Upset Speed: " + (blockContent?.speedUpset ?? "") }
            Label { text: "Draw Speed: " + (blockContent?.speedDraw ?? "") }
        }
    }
}
