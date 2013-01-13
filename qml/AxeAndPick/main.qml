import QtQuick 2.0



Item {
    width: 800
    height: 600
    id: rootWindow

    //var darkGrayColor = "#FFEFEFEF";

    Item {
        id: mainToolbar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        Rectangle {
            anchors.fill: parent
            color: "lightgray"
        }
        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom

            Column {
                spacing: 3
                anchors.top: parent.top
                anchors.topMargin: 5

                Text {
                    id: saveGameName
                    text: "Save Game Name"
                    font.pixelSize: 18
                }
                Text {
                    id: saveGameDate
                    text: "January 01 23:59 PM"
                    font.pixelSize: 10
                    font.italic: true
                    color: "gray"
                }
            }
            Rectangle {
                // This is just a spacer.
                color: "transparent"
                width: 30
                height: 1
            }
            ToolbarTextInput {
                textLabel: "Day"
                value: 6
                width: 45
            }
            ToolbarTextInput {
                textLabel: "Hour"
                value: 23
                width: 38
            }
        }

        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom

            ToolbarImageButton {
                icon: "images/openIcon.svg"
                color: "transparent"
            }
            ToolbarImageButton {
                icon: "images/saveIcon.svg"
                color: "transparent"
            }
        }

    }

    Item {
        id: resourceContainer
        anchors.top: mainToolbar.bottom
        anchors.left: parent.left
        anchors.right: containerSeperator.left
        anchors.bottom: parent.bottom

        // Background for the resource container
        Rectangle {
            anchors.fill: parent
            color: "silver"
        }

        Item {
            id: resourceToolbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30

            Rectangle {
                anchors.fill: parent
                color: "silver"
            }

            SearchBox {
                id: resourceSearchBox
                target: resourceModelProxy
                anchors.right: parent.right

                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ListView {
            id: resourceList
            anchors.top: resourceToolbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.bottom: parent.bottom

            model: resourceModelProxy
            delegate: resourceDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }
        ScrollBar {
            target: resourceList
            width: 16
        }
    }

    Item {
        id: containerSeperator
        anchors.top: mainToolbar.bottom
        anchors.bottom: parent.bottom
        x: 300
        width: 5

        Rectangle {
            anchors.fill: parent
            color: "silver"
        }
        MouseArea {
            anchors.fill: parent
            drag.target: containerSeperator

            // Restrict how far you can drag the seperator.
            drag.minimumX: 10
            drag.maximumX: rootWindow.width - 10

            cursorShape: Qt.SizeHorCursor
        }
    }

    Item {
        id: unitContainer
        anchors.top: mainToolbar.bottom
        anchors.left: containerSeperator.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        Rectangle {
            anchors.fill: parent
            color: "green"
        }
    }



    // This is the component that displays each resource in the list
    Component {
        id: resourceDelegate

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
                    source: "images/" + icon
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

                        font.family: "Helvetica"
                        font.pointSize: 10
                        clip: true
                    }
                    Text {
                        id: typeText
                        text: type

                        anchors.top: nameText.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right

                        font.family: "Helvetica"
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
                                    resourceModel.setData(identification, 0)
                                }
                                else {
                                    resourceModel.setData(identification, quantity-newQuantity)
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
                                resourceModel.setData(identification, parseInt(text,10))
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
                                    resourceModel.setData(identification, 0)
                                }
                                else {
                                    resourceModel.setData(identification, quantity+newQuantity)
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






//    Flickable{
//        anchors.top: searchBar.bottom

//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom

//        clip: true

//        ListView {
//            id: resourceView
//            anchors.fill: parent
//            anchors.rightMargin: 20

//            model: resourceModelProxy
//            delegate: resourceDelegate
//        }
//        ScrollBar {
//            id: resourceScrollBar
//            target: resourceView
//        }
//    }

}


