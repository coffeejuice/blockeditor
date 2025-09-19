pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: headerRect

    property var doc: root.blocksModel.documentBegin

    height: headerCountour.height + 10
    width: parent ? parent.width : 0
    z: blocksView.z + 2

    Rectangle {
        id: headerCountour
        height: headerColumn.height + 12
        width: parent.width - 10
        x: 5
        y: 5
        radius: 5
        border.color: "gray"

        Column {
            id: headerColumn
            anchors.centerIn: parent

            spacing: 6
            Label { text: "Document"; font.bold: true }
            // Safely read fields from the QVariantMap
            Text { text: "Test Text object" }
            Text { text: doc && doc.documentNumber ? "Doc No: " + doc.documentNumber : "" }
            Text { text: doc && doc.material ? "Material: " + doc.material : "" }
            Text { text: doc && doc.meshDensity ? "Mesh: " + doc.meshDensity : "" }
        }
    }
}

