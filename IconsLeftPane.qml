// IconsLeftPane.qml
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import cardmix

Item {
    id: root

    height: parent.height
    width: 50

    required property AbstractItemModel blocksModel
    required property string title
    required property bool viewInteraction

    ListView {
        id: iconsView

        anchors.fill: parent
        spacing: 8
        clip: true
        orientation: ListView.Vertical

        property bool isEnab: root.viewInteraction
        property int lastItem: itemAtIndex(count - 1) ? (itemAtIndex(count - 1).height ? itemAtIndex(count - 1).height : 0) : 0

        signal requestMove(int sourceIndex, int destinationIndex)

        signal requestCopy(int sourceIndex, int destinationIndex)

        Component {
            id: button
            MouseArea {
                height: 40
                width: root.width
                Rectangle {
                    x: 2
                    y: 2
                    height: parent.width - 4
                    width: parent.width - 4
                    color: "gray"
                }
                Image {
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: 18
                    width: 18
                    y: 2
                    source: Qt.resolvedUrl("../assets/" + file_name)
                    smooth: true
                    antialiasing: true
                }
            }
        }

        model: root.iconsModel
        delegate: button

        highlightFollowsCurrentItem: true
        highlight: blocksHighlight
        highlightMoveDuration: 1
        highlightResizeDuration: 1
        focus: root.viewInteraction
    }
    Component {
        id: blocksHighlight

        Rectangle {
            id: highlightRect

            border.color: "green"
            // color: "yellow"
            opacity: 0.0
            z: root.viewInteraction ? (iconsView.z + 2) : 0
        }
    }
}
