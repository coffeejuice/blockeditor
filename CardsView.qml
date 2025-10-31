import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    required property CardsModel model

    ColumnLayout {
        anchors.fill: parent
        spacing: 8

        RowLayout {
            Layout.fillWidth: true
            Label {text: "Cards"; font.bold: true}
            Button {text: "Append"; onClicked: lv.model.appendCard("New Card")}
            Button {text: "Remove"; enabled: lv.currentIndex >= 0; onClicked: lv.model.removeCard(lv.currentIndex)}
        }

        ListView {
            id: lv
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 12
            clip: true
            model: root.model
            delegate: Item {
                id: card
                width: ListView.view.width
                // size to content
                implicitHeight: contentCol.implicitHeight + 20  // 10 top + 10 bottom
                // Let the Column compute content height, then add frame paddings

                // simple frame look
                Rectangle {
                    anchors.fill: parent
                    color: "transparent"
                    radius: 6
                    border.width: 1
                    border.color: "#cccccc"
                }

                Column {
                    id: contentCol
                    anchors.fill: parent
                    anchors.margins: 8
                    spacing: 8
                    // width: card.width - (card.leftPadding + card.rightPadding)

                    Row {
                        id: headerRow
                        spacing: 8
                        Label { text: title; Layout.fillWidth: true; font.pixelSize: 18; font.bold: true }
                        Button { text: "Add Chunk"; onClicked: chunks.appendChunk()}
                        Button { text: "Remove Last"; onClicked: if (chunks.rowCount() > 0) chunks.removeChunk(chunks.rowCount() - 1) }
                    }
                    // FLOW of chunks (each chunk is a Row of variable number of fields)
                    Flow {
                        id: flow
                        width: contentCol.width
                        spacing: 8
                        // One Row per chunk
                        Repeater {
                            model: chunks
                            delegate: Row {
                                property int chunkIndex: index // capture outer chunk index if you need it
                                spacing: 6
                                // For this chunk create editors for each text field
                                Repeater {
                                    id: fieldRepeater
                                    model: fields // FieldsListModel* (role name fields)
                                    delegate: Row {
                                        spacing: 4
                                        Label { text: label + ":" }
                                        TextField {
                                            id: editor
                                            text: String(value) // show as string
                                            readOnly: !editable
                                            onEditingFinished: {
                                                // write back to the FieldListModel (the 'fields' object)
                                                fields.setData(index, editor.text, FieldListModel.ValueRole)
                                            }
                                        }
                                    }
                                }
                                // Chunk level control
                                Button { text: "+"; onClicked: fields.append("New", "")}
                                Button { text: "-"; onClicked: fields.remove(fields.rowCount() - 1); enabled: fields.rowCount() >= 0 }
                            }
                        }
                    }
                    // 2) Tell Frame how tall its content is
                    // implicitContentHeight: contentCol.implicitHeight
                }
            }
            highlight: Rectangle {color: Qt.rgba(0,0,0,0.06)}
            focus: true
        }
    }
}
