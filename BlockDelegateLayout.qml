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
        }

        // Corner action row (plus + dots)
        Row {
            id: cornerActions
            spacing: 6
            anchors.top: contourRect.top
            anchors.right: contourRect.right
            anchors.topMargin: 5
            anchors.rightMargin: 5
            z: 2
            visible: contourRect.cornerActionsVisible
            opacity: visible ? 1 : 0
            Behavior on opacity {
                NumberAnimation {
                    duration: 120
                    easing.type: Easing.OutQuad
                }
            }

            Image {
                id: plusIcon
                source: Qt.resolvedUrl("assets/plus.svg")
                width: 24; height: 24
                fillMode: Image.PreserveAspectFit
                smooth: true
                antialiasing: true

                TapHandler {
                    acceptedButtons: Qt.LeftButton
                    gesturePolicy: TapHandler.DragThreshold
                    onTapped: actionPopup.open()
                    onPressedChanged: plusIcon.opacity = pressed ? 0.7 : 1
                    cursorShape: Qt.PointingHandCursor
                }
            }

            Image {
                id: dotsIcon
                source: Qt.resolvedUrl("assets/dots.svg")
                width: 18; height: 18
                fillMode: Image.PreserveAspectFit
                smooth: true
                antialiasing: true

                DragHandler {
                    id: reorderHandler
                    target: null
                    acceptedDevices: PointerDevice.Mouse | PointerDevice.TouchScreen
                    acceptedButtons: Qt.LeftButton
                    grabPermissions: PointerHandler.CanTakeOverFromAnything
                    cursorShape: active ? Qt.ClosedHandCursor : Qt.OpenHandCursor
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

        property bool pointerInside: false
        readonly property bool cornerActionsVisible: pointerInside || actionPopup.visible || reorderHandler.active

        MouseArea {
            id: interactionArea
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
            onEntered: if (enabled) contourRect.pointerInside = true
            onExited: if (enabled) contourRect.pointerInside = false
            onCanceled: contourRect.pointerInside = false
            onEnabledChanged: if (!enabled) contourRect.pointerInside = false
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

    Drag.active: reorderHandler.active
    Drag.source: root
    Drag.hotSpot.x: reorderHandler.centroid.position.x
    Drag.hotSpot.y: reorderHandler.centroid.position.y
    Drag.mimeData: ({ "application/x-item-index": root.itemIndex })
    Drag.dragType: Drag.Automatic
    Drag.supportedActions: Qt.MoveAction | Qt.CopyAction
}
