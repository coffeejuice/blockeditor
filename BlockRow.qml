// BlockRow.qml
import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: row
    required property var innerContent
    property string imageIndex: innerContent?.image ?? ""

    width: parent ? parent.width : 0
    height: Math.max(60, rightColumn.implicitHeight) + 10
    radius: 5
    border.color: "gray"

    // Left image
    Rectangle {
        id: left
        width: 50; height: 50
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.margins: 5
        color: "black"
        Image {
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            source: Qt.resolvedUrl("assets/image" + row.imageIndex + ".jpg")
        }
    }

    // Right content
    Rectangle {
        id: right
        anchors.left: left.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        color: "#333"
        border.color: Qt.lighter(color, 1.2)

        Column {
            id: rightColumn
            spacing: 6
        }
    }

    // Anything you put inside BlockRow becomes a child of rightColumn
    default property alias details: rightColumn.data
}
