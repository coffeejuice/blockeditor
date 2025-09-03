import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import blockeditor 1.0

Window {
    width: 640; height: 480; visible: true
    title: qsTr("Hello World")

    SelectionController { id: selection }

    ListView {
        id: list
        anchors.fill: parent
        model: blockModel
        spacing: 8
        cacheBuffer: 2000
        focus: true

        delegate: Item {
            id: block
            width: list.width
            implicitHeight: contentWrap.implicitHeight + 12

            // Use role names directly
            property int rowIndex: index
            property int indentLevel: indent        // role: "indent"
            property int btype: blockType           // role: "blockType"

            // Content
            Item {
                id: contentWrap
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 6
                anchors.leftMargin: 18 * indentLevel
                implicitHeight: loader.implicitHeight

                Loader {
                    id: loader
                    anchors.left: parent.left
                    anchors.right: parent.right
                    sourceComponent:
                        (btype === BlockModel.Image)   ? imageBlock :
                                                         textBlock     // Todo & Heading use textBlock too

                    onLoaded: {
                        if (!item) return;
                        item.rowIndex  = index;
                        item.blockType = btype;
                        item.indent    = indentLevel;
                        if ("textValue"     in item) item.textValue     = text;     // role "text"
                        if ("imageUrlValue" in item) item.imageUrlValue = imageUrl; // role "imageUrl"
                    }

                }


            }

            // ---------- Selection handling ----------
            MouseArea {
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton
                preventStealing: true
                onPressed: function(mouse) {
                    if (loader.item && loader.item.apiTextLength !== undefined) {
                        loader.item.forceActiveFocus()
                        const pos = loader.item.apiPositionAt(mouse.x - contentWrap.x, mouse.y - contentWrap.y)
                        selection.setRange(rowIndex, pos, rowIndex, pos)
                    } else {
                        selection.clear()
                    }
                }
                onPositionChanged: function(mouse) {
                    if (!(mouse.buttons & Qt.LeftButton)) return;
                    if (selection.startIndex < 0) return;
                    if (loader.item && loader.item.apiTextLength !== undefined) {
                        const pos = loader.item.apiPositionAt(mouse.x - contentWrap.x, mouse.y - contentWrap.y)
                        selection.setRange(selection.startIndex, selection.startPos, rowIndex, pos)
                    } else {
                        selection.setRange(selection.startIndex, selection.startPos, rowIndex, 1)
                    }
                }
                onReleased: selection.norm()
            }

            Connections {
                target: selection
                function onSelectionChanged() {
                    if (!loader.item) return;
                    if (loader.item.apiSelectRange)
                        loader.item.apiSelectRange(selection.startIndex, selection.startPos,
                                                   selection.endIndex,   selection.endPos,
                                                   rowIndex)
                }
            }
        }

        // Keyboard expand (Shift+Up/Down across delegates)
        Keys.onPressed: function(ev) {
            if (!(ev.modifiers & Qt.ShiftModifier)) return;
            if (ev.key === Qt.Key_Down || ev.key === Qt.Key_Up) {
                const cur = selection.endIndex >= 0 ? selection.endIndex : currentIndex
                let next = cur + (ev.key === Qt.Key_Down ? 1 : -1)
                next = Math.max(0, Math.min(count - 1, next))
                selection.setRange(selection.startIndex >= 0 ? selection.startIndex : cur,
                                   selection.startPos, next, 0)
                ev.accepted = true
            }
        }
    }

    // ---------- Components ----------

    // Text block
    Component {
        id: textBlock
        TextArea {
            id: ta

            // receive values from Loader.onLoaded
            property int    rowIndex: -1
            property int    blockType: 0
            property int    indent: 0
            property string textValue: ""
            property url    imageUrlValue: ""   // not used here, but harmless

            text: textValue
            wrapMode: TextEdit.Wrap
            font.family: "Consolas"; font.pointSize: 12
            implicitHeight: Math.max(120, contentHeight + topPadding + bottomPadding)

            onTextChanged: {
                if (rowIndex >= 0)
                    blockModel.setData(blockModel.index(rowIndex, 0),
                                        text,
                                        BlockModel.TextRole)
            }

            // Selection API (unchanged)
            property int apiTextLength: ta.length
            function apiPositionAt(x, y) { return ta.positionAt(x, y) }
            function apiSelectRange(sIndex, sPos, eIndex, ePos, myIndex) {
                ta.deselect()
                if (selection.startIndex < 0) return
                var from = 0, to = 0
                if (sIndex === myIndex && eIndex === myIndex) {
                    from = Math.min(sPos, ePos); to = Math.max(sPos, ePos)
                } else if (sIndex === myIndex) {
                    from = sPos; to = ta.length
                } else if (eIndex === myIndex) {
                    from = 0;    to = ePos
                } else if (myIndex > Math.min(sIndex, eIndex) && myIndex < Math.max(sIndex, eIndex)) {
                    from = 0; to = ta.length
                } else return
                ta.select(from, to)
            }
        }
    }


    // Image block
    Component {
        id: imageBlock
        Item {
            id: imgRoot

            // receive values from Loader.onLoaded
            property int  rowIndex: -1
            property int  blockType: 0
            property int  indent: 0
            property url  imageUrlValue: ""

            implicitHeight: header.height + 8 + img.height

            Row {
                id: header
                height: 32; spacing: 8
                Button { text: "Change"; onClicked: fileDlg.open() }
                Label  { text: imageUrlValue ? imageUrlValue.toString() : "(no image)" }
            }

            Image {
                id: img
                anchors.top: header.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.topMargin: 8
                source: imageUrlValue || ""      // <-- use the passed-in prop
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                smooth: true
                width: parent.width
                height: (sourceSize.width > 0)
                        ? Math.max(160, sourceSize.height * width / sourceSize.width)
                        : 240
            }

            // selection API stubs
            function apiPositionAt(x,y) { return 0 }
            function apiSelectRange() { }
        }
    }


    // Toolbar (optional)
    ToolBar {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Row {
            anchors.margins: 8
            anchors.left: parent.left
            spacing: 8
            Button { text: "Text";  onClicked: blockModel.appendParagraph("New block") }
            Button { text: "Image"; onClicked: fileDlg.open() }
        }
    }

    FileDialog {
        id: fileDlg
        title: "Choose image"
        onAccepted: blockModel.appendImage(selectedFile)
    }
}
