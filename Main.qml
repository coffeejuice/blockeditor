pragma Strict

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: qsTr("Hello Layouts")
    color: "black"
    palette.windowText: "white"

    property int currentImageIndex: 0
    property var libRef
    // <-- declared so qmllint knows it exists
    Component.onCompleted: libRef = lib // copy C++ context property here

    Component {
        id: thumbnailDelegate // A common Image delegate with a Text which will be used in each example below
        Item {
            id: d

            required property int index
            // Model roles coming from C++ model:
            required property int opIndex
            required property string label

            Layout.preferredWidth: 400
            Layout.preferredHeight: 100

            // Auto-size to content
            // implicitWidth: content.implicitWidth + padding * 2
            // implicitHeight: content.implicitHeight + padding * 2
            RowLayout {

                Image {
                    Layout.preferredWidth: 100
                    Layout.preferredHeight: 100
                    source: Qt.resolvedUrl(`assets/image${d.index}.jpg`)
                }
                Text {
                    font.pixelSize: 30
                    style: Text.Outline
                    styleColor: "black"
                    text: d.opIndex + ":" + d.label
                }
            }

            Rectangle {
                anchors.fill: parent
                color: "transparent"
                radius: 5
                border {
                    color: "red"
                    width: window.currentImageIndex === d.index ? 2 : 0
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: window.currentImageIndex = d.index
            }
        }
    }

    RowLayout {
        anchors {
            fill: parent
            margins: 5
        }

        Flickable {
            id: flickable

            contentHeight: gridLayout.height
            contentWidth: gridLayout.width

            Layout.fillHeight: true // ensure the Flickable fills the height of the RowLayout
            Layout.preferredWidth: gridLayout.width // ensure the Flickable width matches the gridLayout's implicit (two column) width

            clip: true

            ColumnLayout {
                id: gridLayout

                Repeater {
                    model: window.libRef
                    delegate: thumbnailDelegate
                }
            }
        }

        Image {
            id: previewImage
            Layout.fillWidth: true
            Layout.fillHeight: true
            source: Qt.resolvedUrl(
                        `assets/image${window.currentImageIndex}.jpg`)
            asynchronous: true
        }
    }
}
