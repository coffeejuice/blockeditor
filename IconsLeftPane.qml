// IconsLeftPane.qml
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import cardmix

Item {
    id: root

    height: parent.height
    width: 50

    required property AbstractItemModel iconsModel
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
                id: maID

                required property string buttonFile
                required property string buttonLabel

                height: 35
                width: root.width
                Rectangle {
                    x: 1; y: 1
                    height: parent.width - 2
                    width: parent.width - 2
                    radius: 2
                    color: "tranparent"
                    border:

                    Image {
                        id: buttonImage
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: 1
                        height: 18
                        width: 18
                        source: Qt.resolvedUrl("../assets/" + maID.buttonFile)
                        smooth: true
                        antialiasing: true
                    }

                    Label {
                        anchors.horizontalCenter: parent.horizontalCenter
                        y: 1 + 18 + 2
                        text: qsTr(maID.buttonLabel)
                    }
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

            border.color: "gray"
            // color: "yellow"
            opacity: 0.0
            z: root.viewInteraction ? (iconsView.z + 2) : 0
        }
    }
}
