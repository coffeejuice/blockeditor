pragma Strict

pragma ComponentBehavior: Bound

import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import blockeditor

ApplicationWindow {
    id: window
    width: 1280
    height: 800
    visible: true
    title: qsTr("Hello Layouts")

    required property BlocksListModel blocksListModel
    property int currentImageIndex: 0

    color: "black"
    palette.windowText: "white"

    // property var libRef
    // <-- declared so qmllint knows it exists
    // Component.onCompleted: libRef = lib // copy C++ context property here

    // Component {
    //     id: thumbnailDelegate // A common Image delegate with a Text which will be used in each example below
    //     Item {
    //         id: d

    //         required property int index
    //         // Model roles coming from C++ model:
    //         required property int opIndex
    //         required property string label

    //         Layout.preferredWidth: 400
    //         Layout.preferredHeight: 100

    //         // Auto-size to content
    //         // implicitWidth: content.implicitWidth + padding * 2
    //         // implicitHeight: content.implicitHeight + padding * 2
    //         RowLayout {

    //             Image {
    //                 Layout.preferredWidth: 100
    //                 Layout.preferredHeight: 100
    //                 source: Qt.resolvedUrl(`assets/image${d.index}.jpg`)
    //             }
    //             Text {
    //                 font.pixelSize: 30
    //                 style: Text.Outline
    //                 styleColor: "black"
    //                 text: d.opIndex + ":" + d.label
    //             }
    //         }

    //         Rectangle {
    //             anchors.fill: parent
    //             color: "transparent"
    //             radius: 5
    //             border {
    //                 color: "red"
    //                 width: window.currentImageIndex === d.index ? 2 : 0
    //             }
    //         }

    //         MouseArea {
    //             anchors.fill: parent
    //             onClicked: window.currentImageIndex = d.index
    //         }
    //     }
    // }


    // The delegate for each section header
    Component {
        id: sectionHeading
        Rectangle {
            id: rectSection
            width: libraryListView.width
            height: childrenRect.height
            color: "lightsteelblue"

            required property string section

            Text {
                text: rectSection.section
                font.bold: true
                font.pixelSize: 30
            }
        }
    }




    Component {
        id: listViewDelegate

        RowLayout {
            id: listViewLayout

            ListView.onPooled: rotationAnimation.pause() // If an item has timers or animations, consider pausing them on receiving the ListView::pooled signal.
            ListView.onReused: rotationAnimation.resume() // Avoid storing any state inside a delegate. If you do, reset it manually on receiving the ListView::reused signal.

            // required property var model
            required property int index
            required property bool hidden
            required property string name
            required property string number
            required property bool channelActivated

            enabled: !hidden
            visible: !hidden

            width: libraryListView.width
            height: enabled ? implicitHeight : 0
            // height: implicitHeight

            Rectangle {
                id: animatedRectangle
                width: 50
                height: 50
                color: "transparent"

                Rectangle {
                    id: rect
                    anchors.centerIn: parent
                    width: 40
                    height: 5
                    color: "green"

                    RotationAnimation {
                        id: rotationAnimation
                        target: rect
                        duration: (Math.random() * 2000) + 200
                        from: 0
                        to: 359
                        running: true
                        loops: Animation.Infinite
                    }
                }
            }

            ColumnLayout {
                id: contactLayout
                CheckDelegate {
                    checked: listViewLayout.channelActivated
                    text: qsTr("Record %1 / %2").arg(listViewLayout.index + 1).arg(libraryListView.count)
                    onClicked: listViewLayout.hidden = true
                }

                Text {
                    text: `<b>Name:</b>` + listViewLayout.name
                    color: listViewLayout.ListView.isCurrentItem ? "red" : "black"
                }
                Text {
                    text: `<b>Number:</b>` + listViewLayout.number
                    color: listViewLayout.ListView.isCurrentItem ? "red" : "black"
                }
            }
        }
    }



    RowLayout {
        id: mainLayout
        property int margin: 5
        anchors {
            fill: parent
            margins: margin
        }

        Rectangle {
            id: libraryBackground
            Layout.preferredWidth: 200
            Layout.preferredHeight: window.height - mainLayout.margin * 2
            color: "darkgray"
            radius: 5

            ListView {
                id: libraryListView
                anchors.fill: parent

                model: ContactModel {}
                delegate: listViewDelegate
                header: Rectangle {
                    width: libraryListView.width
                    height: 50
                    color: "red"
                    z: 2
                }
                headerPositioning: ListView.OverlayHeader

                section.property: "size"
                section.criteria: ViewSection.FullString
                section.delegate: sectionHeading

                focus: true
                clip: true
                reuseItems: true
                // highlightFollowsCurrentItem: true
                // highlightMoveDuration: 10  // milliseconds

                highlight: Rectangle {
                    color: "lightsteelblue"
                    radius: 5
                    border {
                        color: "steelblue"
                        width: 1
                    }
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
