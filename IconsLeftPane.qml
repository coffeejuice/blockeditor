// BlocksListView.qml
pragma ComponentBehavior: Bound
import QtQuick
import QtQuick.Controls.Basic

import cardmix

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

        model: root.blocksModel

        delegate: DelegateChooser {
            role: "type" // C++ must expose this role

            DelegateChoice { roleValue: "document";    delegate: Card_Document {} }
            DelegateChoice { roleValue: "block";       delegate: Card_Block {}}
            DelegateChoice { roleValue: "heat";        delegate: Card_Heat {} }
            DelegateChoice { roleValue: "upset";       delegate: Card_Upset {} }
            DelegateChoice { roleValue: "draw";        delegate: Card_Draw {} }
            // optional fallback
            DelegateChoice { delegate: Card_Unknown {} }
        }

        header: Card_Header {}
        headerPositioning: ListView.OverlayHeader

        footer: Card_Footer {}

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
            if (iconsView.currentIndex >= 0) {
                let index = iconsView.model.index(iconsView.currentIndex, 0)
                iconsView.model.removeRows(iconsView.currentIndex, 1, index.parent)
            }
        }

        Keys.onEscapePressed: {
            iconsView.currentIndex = -1
        }

        Keys.onSpacePressed: {
            if (iconsView.currentIndex >= 0) {
                let index = iconsView.model.index(iconsView.currentIndex, 0)
                iconsView.model.setData(index, { "type": "draw", "content": { "operations": "(666)->595->(90)620" } }, Qt.EditRole)
            }
        }

        Keys.onDigit1Pressed: {
            newValuePopup.open()
        }

        Keys.onDigit2Pressed: {
            iconsView.model.clearModel(iconsView.count)
        }

        Keys.onUpPressed: {
            if (iconsView.count > 1 || iconsView.currentIndex > 0) {
                if (iconsView.model.moveRowTo(iconsView.currentIndex, iconsView.currentIndex - 1)) {
                    iconsView.currentIndex = Math.max(0, iconsView.currentIndex - 1)
                }
            }
        }

        Keys.onDownPressed: {
            if (iconsView.count > 1 || iconsView.currentIndex < (iconsView.count - 1)) {
                if (iconsView.model.moveRowTo(iconsView.currentIndex, iconsView.currentIndex + 1)) {
                    iconsView.currentIndex = Math.min(iconsView.count - 1, iconsView.currentIndex + 1)
                }
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
                        iconsView.model.appendRow(iconsView.count)
                        let index = iconsView.model.index(iconsView.count - 1, 0)
                        iconsView.model.setData(index, "%1, %2, %3, %4"
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

            border.color: "green"
            // color: "yellow"
            opacity: 0.0
            z: root.viewInteraction ? (iconsView.z + 2) : 0
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
