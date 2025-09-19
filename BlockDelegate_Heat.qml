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
    imageIndex: "fire.svg"

    Column {
        id: heatColumn
        spacing: 6
        Label { text: "Heat"; font.bold: true }
        Label { text: "Units: " + (blockContent?.timeUnits ?? "") }
        Text {
            text: String(blockContent?.typeTimeTemperature ?? "")
            wrapMode: Text.Wrap
        }
    }
}
