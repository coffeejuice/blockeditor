pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import blockeditor

/*
Controls
    Arrow keys: move the current element up and down
    Escape: de-selects the current element
    Delete: deletes the current element
    Tab: inserts a copy of the current element
    Space: sets the current element to "Greg Doe"
    1: opens popup to insert a new entry
    2: deletes entire model
 */

Item {
    id: root

    height: parent.height
    width: parent.width

    required property AbstractItemModel blocksModel
    required property string title
    required property bool viewInteraction

    ListView {
        id: blocksView

        anchors.fill: parent
        spacing: 8
        clip: true
        orientation: ListView.Vertical

        model: root.blocksModel
        // delegate: blocksDelegate

        delegate: DelegateChooser {
            role: "blockType" // C++ must expose this role

            DelegateChoice { roleValue: "documentBegin";    delegate: documentBeginDelegate }
            DelegateChoice { roleValue: "blockBegin";       delegate: blockBeginDelegate }
            DelegateChoice { roleValue: "heat";             delegate: heatDelegate }
            DelegateChoice { roleValue: "upset";            delegate: upsetDelegate }
            DelegateChoice { roleValue: "draw";             delegate: drawDelegate }
        }

        header: blocksHeader
        footer: blocksFooter


        highlightFollowsCurrentItem: true
        highlight: blocksHighlight
        highlightMoveDuration: 1
        highlightResizeDuration: 1
        focus: root.viewInteraction

        displaced: displacedTransition
        add: addTransition
        remove: removeTransition
        move: moveTransition

        Keys.onDeletePressed: {
            if (blocksView.currentIndex < 0) {
                console.log("Must select an element to delete an entry")
            } else {
                let index = blocksView.model.index(blocksView.currentIndex, 0)
                blocksView.model.removeRows(blocksView.currentIndex, 1, index.parent)
            }
        }

        Keys.onEscapePressed: {
            blocksView.currentIndex = -1
        }

        Keys.onSpacePressed: {
            if (blocksView.currentIndex < 0) {
                console.log("Must select an element to modify an entry")
            } else {
                let index = blocksView.model.index(blocksView.currentIndex, 0)
                blocksView.model.setData(index, { "type": "draw", "content": { "operations": "(666)->595->(90)620" } }, Qt.EditRole)
            }
        }

        Keys.onDigit1Pressed: {
            newValuePopup.open()
        }

        Keys.onDigit2Pressed: {
            blocksView.model.clearModel(blocksView.count)
        }

        Keys.onUpPressed: {
            if (blocksView.count <= 1) {
                console.log("Not enough elements in model to move an element")
            } else if (blocksView.currentIndex === 0){
                console.log("Cannot move first element up")
            } else {
                let sourceIndex = blocksView.model.index(blocksView.currentIndex, 0)
                let destinationIndex = blocksView.model.index(blocksView.currentIndex - 1, 0)
                blocksView.model.moveRows(sourceIndex.parent, blocksView.currentIndex, 1, destinationIndex.parent, blocksView.currentIndex - 1)
            }
        }

        Keys.onDownPressed: {
            if (blocksView.count <= 1) {
                console.log("Not enough elements in model to move an element")
            } else if (blocksView.currentIndex === (blocksView.count - 1)){
                console.log("Cannot move last element down")
            } else {
                let sourceIndex = blocksView.model.index(blocksView.currentIndex, 0)
                let destinationIndex = blocksView.model.index(blocksView.currentIndex + 1, 0)
                blocksView.model.moveRows(sourceIndex.parent, blocksView.currentIndex, 1, destinationIndex.parent, blocksView.currentIndex + 1)
            }
        }

        // ========================== POP UP ===============================

        Popup {
            id: newValuePopup

            height: 250
            width: 200
            anchors.centerIn: parent

            Column {
                anchors.centerIn: parent
                spacing: 5

                TextField {
                    id: firstNameField

                    font.pointSize: 10
                    placeholderText: qsTr("Enter first name")
                    width: 150
                    height: 30
                }

                TextField {
                    id: lastNameField

                    font.pointSize: 10
                    placeholderText: qsTr("Enter last name")
                    width: 150
                    height: 30
                }

                TextField {
                    id: ageField

                    font.pointSize: 10
                    placeholderText: qsTr("Enter age")
                    width: 150
                    height: 30
                    validator: IntValidator{}
                }

                TextField {
                    id: phoneField

                    font.pointSize: 10
                    placeholderText: qsTr("Enter phone number")
                    width: 150
                    height: 30
                    validator: RegularExpressionValidator{regularExpression: /^\d{3}-\d{3}-\d{4}$/}
                }

                Button {
                    height: 25
                    width: firstNameField.width
                    text: qsTr("Append")
                    anchors.horizontalCenter: parent.horizontalCenter
                    enabled: ((firstNameField.text !== "") && (lastNameField.text !== "") && (ageField.text !== "") && (phoneField.text !== ""))

                    onClicked: {
                        blocksView.model.appendRow(blocksView.count)
                        let index = blocksView.model.index(blocksView.count - 1, 0)
                        blocksView.model.setData(index, "%1, %2, %3, %4"
                                                   .arg(firstNameField.text).arg(lastNameField.text)
                                                   .arg(ageField.text).arg(phoneField.text))
                        newValuePopup.close()
                        firstNameField.text = ""
                        lastNameField.text = ""
                        ageField.text = ""
                        phoneField.text = ""
                    }
                }
            }
        }
    }


    Component {
        id: blocksDelegate

        Rectangle {
            id: delegateRect

            required property var blockContent
            property string imageIndex: delegateRect.blockContent.image
            property ItemView parentView: ListView.view

            width: parent ? parent.width : 0
            height: gridView.implicitHeight + 5
            radius: 5
            border.color: "gray"

            Rectangle {
                id: image

                width: 50
                height: 50

                anchors.left: parent.left
                anchors.top: parent.top
                anchors.leftMargin: 5
                anchors.topMargin: 5

                color: "black"

                Image {
                    anchors.fill: parent

                    fillMode: Image.PreserveAspectFit

                    source: Qt.resolvedUrl("assets/image" + delegateRect.imageIndex + ".jpg")  // `assets/image${delegateRect.imageIndex}.jpg`)
                }
            }

            Rectangle {
                anchors.left: image.left
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.leftMargin: 5
                anchors.topMargin: 5
                anchors.rightMargin: 5

                height: gridView.implicitHeight

                color: "#333"
                border.color: Qt.lighter(color, 1.2)

                ListView {
                    id: gridView

                    anchors.fill: parent
                    clip: true
                    // cellWidth: parent.implicitWidth
                    // cellHeight: 20

                    model: root.blocksModel

                    delegate: DelegateChooser {
                        role: "blockType" // C++ must expose this role

                        DelegateChoice { roleValue: "documentBegin";    delegate: documentBeginDelegate }
                        DelegateChoice { roleValue: "blockBegin";       delegate: blockBeginDelegate }
                        DelegateChoice { roleValue: "heat";             delegate: heatDelegate }
                        DelegateChoice { roleValue: "upset";            delegate: upsetDelegate }
                        DelegateChoice { roleValue: "draw";             delegate: drawDelegate }
                    }
                }
            }

            // MouseArea {
            //     anchors.fill: parent
            //     hoverEnabled: true
            //     enabled: root.viewInteraction

            //     onReleased: {
            //         delegateRect.blocksView.currentIndex = delegateRect.index
            //         delegateRect.color = "white"
            //     }

            //     onPressed: {
            //         delegateRect.color = "gray"
            //     }

            //     onEntered: {
            //         delegateRect.color = "lightgray"
            //     }

            //     onExited: {
            //         delegateRect.color = "white"
            //     }
            // }

            // Keys.onTabPressed: {
            //     if (blocksView.currentIndex < 0) {
            //         console.log("Must select an element to insert a new entry")
            //     } else {
            //         let index = blocksView.model.index(blocksView.currentIndex, 0)
            //         blocksView.model.insertRows(blocksView.currentIndex, 1, index.parent)
            //         blocksView.model.setData(index, "%1, %2, %3, %4"
            //                                    .arg(delegateRect.firstName).arg(delegateRect.lastName)
            //                                    .arg(delegateRect.age).arg(delegateRect.phoneNumber))
            //     }
            // }
        }
    }

    // ====== Reusable item UIs ======
    Component {
        id: documentBeginDelegate
        BlockRow {
            required property var blockContent
            innerContent: blockContent
            Column {
                Label { text: "Document"; font.bold: true }
                Label { text: "No: " + (blockContent?.documentNumber ?? "") }
                Label { text: "Material: " + (blockContent?.material ?? "") }
                Label { text: "Mesh density: " + (blockContent?.meshDensity ?? "") }
            }
        }
    }

    Component {
        id: blockBeginDelegate
        BlockRow {
            required property var blockContent
            innerContent: blockContent
            Column {
                id: blockColumn
                spacing: 6
                Label { text: "Begin Block"; font.bold: true }
                Label { text: "Press: " + (blockContent?.press ?? "") }
                Label { text: "Dies: " + (blockContent?.dies ?? "") }
                Row {
                    spacing: 12
                    Label { text: "Upset: " + (blockContent?.speedUpset ?? "") }
                    Label { text: "Draw: " + (blockContent?.speedDraw ?? "") }
                }
            }
        }
    }

    Component {
        id: heatDelegate
        BlockRow {
            required property var blockContent
            innerContent: blockContent
            Column {
                id: heatColumn
                spacing: 6
                Label { text: "Heat"; font.bold: true }
                Label { text: "Units: " + (blockContent?.timeUnits ?? "") }
                Text {
                    text: String(blockContent?.typeTimeTemperature ?? "")
                    wrapMode: Text.Wrap
                }
            }
        }
    }

    Component {
        id: upsetDelegate
        BlockRow {
            required property var blockContent
            innerContent: blockContent
            Column {
                id: upsetColumn
                spacing: 6
                Label { text: "Upset"; font.bold: true }
                Label { text: String(blockContent?.operations ?? "") }
            }
        }
    }

    Component {
        id: drawDelegate
        BlockRow {
            required property var blockContent
            innerContent: blockContent
            Column {
                id: drawColumn
                spacing: 6
                Label { text: "Draw"; font.bold: true }
                Label { text: String(blockContent?.operations ?? "") }
            }
        }
    }

    // Component {
    //     id: unknownDelegate
    //     Frame {
    //         id: unknownFrame

    //         required property var blockContent
    //         property ItemView blocksView: ListView.view

    //         width: parent ? parent.width : 0
    //         padding: 12
    //         Column {
    //             spacing: 6
    //             Label { text: "Unknown type: " + String(unknownFrame.blockContent.type) ; font.bold: true }
    //             Text { text: JSON.stringify(obj, null, 2); font.family: "monospace" }
    //         }
    //     }
    // }

    Component {
        id: blocksHeader

        Rectangle {
            id: headerRect

            height: 35
            width: parent.width
            border.color: "black"
            color: "mistyrose"
            z: blocksView.z + 2

            Text {
                anchors.centerIn: parent
                text: root.title
                font.pointSize: 18
            }
        }
    }

    Component {
        id: blocksFooter

        Rectangle {
            id: footerRect

            height: 20
            width: parent.width
            border.color: "black"
            color: "mistyrose"
        }
    }

    Component {
        id: blocksHighlight

        Rectangle {
            id: highlightRect

            border.color: "black"
            color: "yellow"
            opacity: 0.15
            z: root.viewInteraction ? (blocksView.z + 2) : 0
        }
    }

    Transition {
        id: displacedTransition

        NumberAnimation {
            properties: "x,y"
            duration: 300
        }
    }

    Transition {
        id: addTransition

        NumberAnimation {
            property: "opacity"
            from: 0
            to: 1.0
            duration: 300
        }

        NumberAnimation {
            property: "scale"
            from: 0
            to: 1.0
            duration: 300
        }
    }

    Transition {
        id: moveTransition

        NumberAnimation {
            properties: "x,y"
            duration: 300
        }
    }

    Transition {
        id: removeTransition

        NumberAnimation {
            property: "opacity"
            from: 1.0
            to: 0
            duration: 300
        }

        NumberAnimation {
            property: "scale"
            from: 1.0
            to: 0
            duration: 300
        }
    }
}
