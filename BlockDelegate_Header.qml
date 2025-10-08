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
    imageIndex: "check_list_bw.svg"

    Column {
        id: headerColumn
        spacing: 6
        Label { text: "Document"; font.bold: true }
        Label { text: "Document Number: " + (blockContent?.documentNumber ?? "") }
        Label { text: "Material: " + (blockContent?.material ?? "") }
        Label { text: "Mesh density: " + (blockContent?.meshDensity ?? "") }
    }
}

// Rectangle {
//     id: headerRect

//     property var doc: root.blocksModel.document

//     height: headerCountour.height + 10
//     width: parent ? parent.width : 0
//     z: blocksView.z + 2

//     Rectangle {
//         id: headerCountour
//         height: headerColumn.height + 12
//         width: parent.width - 10
//         x: 5
//         y: 5
//         radius: 5
//         border.color: "gray"

//         Column {
//             id: headerColumn
//             anchors.centerIn: parent

//             spacing: 6
//             Label { text: "Document"; font.bold: true }
//             // Safely read fields from the QVariantMap
//             Text { text: "Test Text object" }
//             Text { text: doc && doc.documentNumber ? "Doc No: " + doc.documentNumber : "" }
//             Text { text: doc && doc.material ? "Material: " + doc.material : "" }
//             Text { text: doc && doc.meshDensity ? "Mesh elements across width: " + doc.meshDensity : "" }
//             Text { text: doc && doc.weight ? "Weight [kg]: " + doc.weight : "" }
//         }
//     }
// }

