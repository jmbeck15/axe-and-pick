import QtQuick 2.0
import Resource 1.0

// This is the component that displays each resource in the list
Component {

    // The box that holds each resource item
    Rectangle {

        color: "#FFeeeeee"
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right

        border.width: 1
        border.color: "#FFE3E3E3"

        // Name and Type and Resource Icon
        Item {
            id: resourceId

            anchors.left: parent.left
            anchors.right: quantityControls.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10

            // Resource Icon
            Image {
                id: resourceIcon
                // TODO: When proper icons are made, this can be changed to:
                // "images/resources/" + name + ".svg"
                source: "images/resources/defaultResourceIcon.svg"
                anchors.verticalCenter: parent.verticalCenter
            }

            // Name and Type
            Item
            {
                id: nameAndType
                anchors.left: resourceIcon.right
                anchors.right: parent.right
                anchors.leftMargin: 8
                anchors.top: resourceIcon.top
                anchors.bottom: resourceIcon.bottom

                Text {
                    id: nameText
                    text: name

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 10
                    clip: true
                }
                Text {
                    id: typeText
                    text: type

                    anchors.top: nameText.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 7
                    color: "grey"
                    clip: true
                }
            }
        }

        // Resource Quantity Buffer
        Item {
            id: quantityControls
            width: 105

            anchors.top: parent.top
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Rectangle {
                id: quantityBoxOutline
                color: "#FF999999"
                height: 19
                width: 85

                anchors.left: parent.left
                anchors.leftMargin: 3
                anchors.verticalCenter: parent.verticalCenter

                Image {
                    id: subtractQuantity
                    source: "images/subtract.svg"
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        id: subtractIconClick
                        anchors.fill: parent

                        onClicked: {
                            var newQuantity;
                            if (mouse.modifiers & Qt.ShiftModifier) {
                                newQuantity = 100;
                            }
                            else {
                                newQuantity = 10;
                            }

                            if ((quantity-newQuantity)<0) {
                                desiredQuantity.text = 0
                            }
                            else {
                                desiredQuantity.text = quantity-newQuantity
                            }
                        }
                    }
                    states: State {
                        name: "mouse-down"
                        when: subtractIconClick.pressed
                        PropertyChanges {
                            target: subtractQuantity
                            source: "images/subtract_click.svg"
                        }
                    }
                }

                Rectangle {
                    id: quantityBoxBackground
                    color: "white"
                    height: parent.height-2
                    clip: true

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: subtractQuantity.right
                    anchors.right: addQuantity.left

                    TextInput {
                        id: desiredQuantity
                        width: parent.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            leftMargin: 8;
                            centerIn: parent;
                        }

                        text: quantity
                        color: desiredQuantity.acceptableInput ? "black" : "red"

                        // Only allow integers, and set the text to
                        // be invalid if there is nothing in the box.
                        // NOTE: Just for fun, really.
                        validator: RegExpValidator{ regExp: /\d+/ }

                        autoScroll: true
                        selectByMouse: true
                        font.pointSize: 10
                        onTextChanged: {
                            // The maximum number of resources is 64k (or near that). This
                            // limits the size so in-game resource collection won't exceed
                            // the save-game limit.
                            if (text > 60000)
                            {
                                text = 60000;
                            }

                            resourceModel.setData(id, parseInt(text,10), Resource.QuantityRole)
                        }
                    }
                }
                Image {
                    id: addQuantity
                    source: "images/add.svg"
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    MouseArea {
                        id: addIconClick
                        anchors.fill: parent
                        onClicked: {
                            var newQuantity;
                            // If Shift+Click, add 100 instead of 10.
                            if (mouse.modifiers & Qt.ShiftModifier) {
                                newQuantity = 100;
                            }
                            else {
                                newQuantity = 10;
                            }

                            if ((quantity+newQuantity)<0) {
                                desiredQuantity.text = 0
                            }
                            else {
                                desiredQuantity.text = quantity+newQuantity
                            }
                        }
                    }
                    states: State {
                        name: "mouse-down"
                        when: addIconClick.pressed
                        PropertyChanges {
                            target: addQuantity
                            source: "images/add_click.svg"
                        }
                    }
                }
            }
        }
    }
}
