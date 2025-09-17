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

    // ====== Reusable item UIs ======
    Component {
        id: documentBeginDelegate
        Frame {
            id: documentBeginId
            // property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Document"; font.bold: true }
                Label { text: "No: " + (list.model?.documentNumber ?? "") }
                Label { text: "Material: " + (list.model?.material ?? "") }
                Label { text: "Mesh density: " + (list.model?.meshDensity ?? "") }
            }
        }
    }

    Component {
        id: blockBeginDelegate
        Frame {
            id: blockBeginId
            property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Begin Block"; font.bold: true }
                Label { text: "Press: " + (blockBeginId.obj?.press ?? "") }
                Label { text: "Dies: " + (blockBeginId.obj?.dies ?? "") }
                Row {
                    spacing: 12
                    Label { text: "Upset: " + (blockBeginId.obj?.speedUpset ?? "") }
                    Label { text: "Draw: " + (blockBeginId.obj?.speedDraw ?? "") }
                }
            }
        }
    }

    Component {
        id: heatDelegate
        Frame {
            id: heatId
            property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Heat"; font.bold: true }
                Label { text: "Units: " + (heatId.obj?.timeUnits ?? "") }
                Text {
                    text: String(heatId.obj?.typeTimeTemperature ?? "")
                    wrapMode: Text.Wrap
                }
            }
        }
    }

    Component {
        id: upsetDelegate
        Frame {
            id: upsetId
            property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Upset"; font.bold: true }
                Label { text: String(upsetId.obj?.operations ?? "") }
            }
        }
    }

    Component {
        id: drawDelegate
        Frame {
            id: drawId
            property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Draw"; font.bold: true }
                Label { text: String(drawId.obj?.operations ?? "") }
            }
        }
    }

    Component {
        id: unknownDelegate
        Frame {
            property var obj: blockContent
            width: parent ? parent.width : 0
            padding: 12
            Column {
                spacing: 6
                Label { text: "Unknown type: " + String(obj.type) ; font.bold: true }
                Text { text: JSON.stringify(obj, null, 2); font.family: "monospace" }
            }
        }
    }

    Component {
        id: blocksDelegate

        Rectangle {
            id: delegateRect

            required property string firstName
            required property string lastName
            required property int age
            required property string phoneNumber
            required property int index
            property ItemView blocksView: ListView.view

            height: 50
            width: parent ? parent.width : 0
            border.color: "black"

            Text {
                anchors.left: parent.left
                anchors.leftMargin: 5
                text: qsTr("Name: %1 %2 \nAge: %3 \nPhone: %4"
                .arg(delegateRect.firstName).arg(delegateRect.lastName)
                .arg(delegateRect.age).arg(delegateRect.phoneNumber))
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true
                enabled: root.viewInteraction

                onReleased: {
                    delegateRect.blocksView.currentIndex = delegateRect.index
                    delegateRect.color = "white"
                }

                onPressed: {
                    delegateRect.color = "gray"
                }

                onEntered: {
                    delegateRect.color = "lightgray"
                }

                onExited: {
                    delegateRect.color = "white"
                }
            }

            Keys.onTabPressed: {
                if (blocksView.currentIndex < 0) {
                    console.log("Must select an element to insert a new entry")
                } else {
                    let index = blocksView.model.index(blocksView.currentIndex, 0)
                    blocksView.model.insertRows(blocksView.currentIndex, 1, index.parent)
                    blocksView.model.setData(index, "%1, %2, %3, %4"
                                               .arg(delegateRect.firstName).arg(delegateRect.lastName)
                                               .arg(delegateRect.age).arg(delegateRect.phoneNumber))
                }
            }
        }
    }

    Component {
        id: contactsHeader

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
        id: contactsFooter

        Rectangle {
            id: footerRect

            height: 20
            width: parent.width
            border.color: "black"
            color: "mistyrose"
        }
    }

    Component {
        id: contactsHighlight

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
