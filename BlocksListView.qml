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

        property bool isEnab: root.viewInteraction
        property int lastItem: itemAtIndex(count - 1) ? (itemAtIndex(count - 1).height ? itemAtIndex(count - 1).height : 0) : 0

        model: root.blocksModel

        delegate: DelegateChooser {
            role: "blockType" // C++ must expose this role

            DelegateChoice { roleValue: "document";    delegate: BlockDelegate_Document {} }
            DelegateChoice { roleValue: "block";       delegate: BlockDelegate_Block {}}
            DelegateChoice { roleValue: "heat";             delegate: BlockDelegate_Heat {} }
            DelegateChoice { roleValue: "upset";            delegate: BlockDelegate_Upset {} }
            DelegateChoice { roleValue: "draw";             delegate: BlockDelegate_Draw {} }
            // optional fallback
            DelegateChoice { delegate: BlockDelegate_Unknown {} }
        }

        header: BlockDelegate_Header {}
        headerPositioning: ListView.OverlayHeader

        footer: BlockDelegate_Footer {}

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
