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

        signal requestMove(int sourceIndex, int destinationIndex)
        signal requestCopy(int sourceIndex, int destinationIndex)

        model: root.blocksModel

        delegate: DelegateChooser {
            role: "type" // C++ must expose this role

            DelegateChoice { roleValue: "document";    delegate: BlockDelegate_Document {} }
            DelegateChoice { roleValue: "block";       delegate: BlockDelegate_Block {}}
            DelegateChoice { roleValue: "heat";        delegate: BlockDelegate_Heat {} }
            DelegateChoice { roleValue: "upset";       delegate: BlockDelegate_Upset {} }
            DelegateChoice { roleValue: "draw";        delegate: BlockDelegate_Draw {} }
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

        // For the example only: perform the move/copy on the QML ListModel.
        // onRequestMove: (sourceIndex, destinationIndex) => {
        //     let fromIndex = Number(sourceIndex)
        //     let toIndex = Number(destinationIndex)
        //     if (!isFinite(fromIndex) || !isFinite(toIndex)) return
        //     if (fromIndex < 0 || toIndex < 0) return
        //
        //     let finalIndex = Math.max(0, Math.min(count - 1, toIndex))
        //     if (finalIndex === fromIndex) return
        //     if (blocksView.model.moveRowTo(fromIndex, finalIndex)) {
        //         blocksView.currentIndex = finalIndex
        //     }
        // }
        // onRequestCopy: (sourceIndex, destinationIndex) => {
        //     if (sourceIndex < 0 || destinationIndex < 0) return
        //     var e = demoModel.get(sourceIndex)
        //     var target = Math.max(0, Math.min(count, destinationIndex))
        //     blocksView.model.insert(target, { text: e.text + " (copy)" })
        // }

        // DropArea that spans the whole list to accept moves/copies.
        // DropArea {
        //     anchors.fill: parent
        //
        //     onDropped: (drop) => {
        //         // Read the source index from mimeData
        //         var from = drop.mimeData["application/x-item-index"]
        //         var fromIndex = Number(from)
        //         if (!isFinite(fromIndex)) {
        //             drop.acceptProposedAction()
        //             return
        //         }
        //
        //         if (blocksView.count === 0) {
        //             drop.acceptProposedAction()
        //             return
        //         }
        //
        //         // Compute destination index from drop position
        //         var y = drop.position.y
        //         var indexAtPos = blocksView.indexAt(0, y)
        //         var insertionIndex
        //         if (indexAtPos < 0) {
        //             insertionIndex = blocksView.count
        //         } else {
        //             var item = blocksView.itemAtIndex(indexAtPos)
        //             if (item) {
        //                 var itemTop = item.y - blocksView.contentY
        //                 var midpoint = itemTop + item.height / 2
        //                 insertionIndex = y > midpoint ? indexAtPos + 1 : indexAtPos
        //             } else {
        //                 insertionIndex = indexAtPos
        //             }
        //         }
        //
        //         // Translate insertion index into final index for the model helper.
        //         var finalIndex = insertionIndex
        //         if (fromIndex < insertionIndex) {
        //             finalIndex = Math.min(blocksView.count - 1, insertionIndex - 1)
        //         } else {
        //             finalIndex = Math.min(blocksView.count - 1, insertionIndex)
        //         }
        //
        //         if (finalIndex < 0) {
        //             drop.acceptProposedAction()
        //             return
        //         }
        //
        //         if (finalIndex === fromIndex) {
        //             drop.acceptProposedAction()
        //             return
        //         }
        //
        //         // Copy when Ctrl held, otherwise move
        //         var isCopy = (drop.keys & Qt.ControlModifier) !== 0
        //         if (isCopy) {
        //             blocksView.requestCopy(fromIndex, finalIndex)
        //         } else {
        //             blocksView.requestMove(fromIndex, finalIndex)
        //         }
        //         drop.acceptProposedAction()
        //     }
        // }

        Keys.onDeletePressed: {
            if (blocksView.currentIndex >= 0) {
                let index = blocksView.model.index(blocksView.currentIndex, 0)
                blocksView.model.removeRows(blocksView.currentIndex, 1, index.parent)
            }
        }

        Keys.onEscapePressed: {
            blocksView.currentIndex = -1
        }

        Keys.onSpacePressed: {
            if (blocksView.currentIndex >= 0) {
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
            if (blocksView.count > 1 || blocksView.currentIndex > 0) {
                if (blocksView.model.moveRowTo(blocksView.currentIndex, blocksView.currentIndex - 1)) {
                    blocksView.currentIndex = Math.max(0, blocksView.currentIndex - 1)
                }
            }
        }

        Keys.onDownPressed: {
            if (blocksView.count > 1 || blocksView.currentIndex < (blocksView.count - 1)) {
                if (blocksView.model.moveRowTo(blocksView.currentIndex, blocksView.currentIndex + 1)) {
                    blocksView.currentIndex = Math.min(blocksView.count - 1, blocksView.currentIndex + 1)
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

            border.color: "green"
            // color: "yellow"
            opacity: 0.0
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
