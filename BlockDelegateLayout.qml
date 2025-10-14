// BlockRow.qml
import QtQuick
import QtQuick.Controls.Basic

Rectangle {
    id: root
    required property var imageIndex
    required property int itemIndex
    required property ItemView listView

    z: 0
    color: ListView.isCurrentItem ? "#3300FF00" : "transparent"  // semi-transparent fill

    width: parent ? parent.width : 0
    height: contourRect.height + 10

    Rectangle {
        id: contourRect

        width: parent.width - 10
        height: Math.max(left.height, rightColumn.height) + 10
        y: 5
        x: 5
        radius: 5
        border.color: "gray"

        // contourRect stays above:
        z: 1

        // Left image
        Rectangle {
            id: left
            width: 50; height: 50; x: 5; y: 5
            color: "transparent"
            Image {
                anchors.fill: parent
                fillMode: Image.PreserveAspectFit
                source: Qt.resolvedUrl("assets/" + root.imageIndex)
                smooth: true
            }

            // Corner action row (plus + dots)
            Row {
                id: cornerActions
                // z: 3
                spacing: 6
                anchors.top: contourRect.top
                anchors.right: contourRect.right
                anchors.topMargin: 5
                anchors.rightMargin: 5
                visible: true // cornerActionsEnabled
                enabled: true // cornerActionsEnabled

                // + icon -> opens popup
                Image {
                    id: plusIcon
                    source: "plus.svg"      // ensure your SVG plugin is available
                    width: 25; height: 25
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    antialiasing: true

                    MouseArea {
                        anchors.fill: parent
                        onClicked: actionPopup.open()
                        hoverEnabled: true
                        cursorShape: Qt.PointingHandCursor
                    }
                }

                // ⋯ icon -> drag handle (move/copy with Ctrl)
                Image {
                    id: dotsIcon
                    source: "dots.svg"
                    width: 10; height: 10
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    antialiasing: true

                    We use DragHandler as the gesture recognizer.
                    DragHandler {
                        id: dotsDrag
                        target: null                // don't physically move the delegate
                        onActiveChanged: {
                            if (active) {
                                // Start QML Drag from the delegate
                                delegateRoot.Drag.supportedActions =
                                    (Qt.application.keyboardModifiers & Qt.ControlModifier)
                                        ? Qt.CopyAction : Qt.MoveAction
                                delegateRoot.Drag.active = true
                            } else {
                                delegateRoot.Drag.active = false
                            }
                        }
                        grabPermissions: PointerHandler.CanTakeOverFromAnything
                        acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchScreen
                    }

                    Attach drag meta to the delegate
                    Drag.active: false
                    Drag.source: delegateRoot
                    Drag.hotSpot.x: width / 2
                    Drag.hotSpot.y: height / 2
                    // Send the source index through mimeData
                    Drag.mimeData: ({ "application/x-item-index": index })
                    Drag.dragType: Drag.Automatic

                    // Visual cue for draggability
                    TapHandler {
                        acceptedButtons: Qt.LeftButton
                        onPressedChanged: if (pressed) dotsIcon.opacity = 0.6
                        onTapped: dotsIcon.opacity = 1.0
                        onCanceled: dotsIcon.opacity = 1.0
                    }
                }
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

        property bool cornerActionsEnabled

        MouseArea {
            z: -1
            anchors.fill: contourRect
            anchors.margins: 0
            hoverEnabled: true
            propagateComposedEvents: true

            // Completely disable reactions when selected
            enabled: root.listView.parent.viewInteraction && (root.listView.currentIndex !== root.itemIndex)
            acceptedButtons: enabled ? Qt.LeftButton | Qt.RightButton : Qt.NoButton
            preventStealing: enabled

            onReleased: if (enabled) {
                root.listView.currentIndex = root.itemIndex
                contourRect.color = "white"
            }
            // onPressed: if (enabled) contourRect.color = "gray"
            onEntered: if (enabled) cornerActionsEnabled = true
            onExited: if (enabled) cornerActionsEnabled = false
        }

        // The popup opened by the plus icon
        Popup {
            id: actionPopup
            x: parent.width - width - 5
            y: cornerActions.y + cornerActions.height + 4
            modal: false
            focus: true
            closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutside

            contentItem: Rectangle {
                implicitWidth: 180
                implicitHeight: 90
                radius: 8
                color: "#ffffff"
                border.color: "#bfc4cc"
                Text {
                    text: "Hello from item " + index
                    anchors.centerIn: parent
                }
            }
        }
    }

    // Anything you put inside BlockRow becomes a child of rightColumn
    default property alias details: rightColumn.data
}
