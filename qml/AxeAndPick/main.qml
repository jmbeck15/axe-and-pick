import QtQuick 2.0

Rectangle {
    width: 360
    height: 460
    color: "#FFEFEFEF"
    id: iAmTheRootObject

    Rectangle {
        id: topAreaBackground

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 40
        color: "lightgray"
    }

    Item {
        id: topArea

        anchors.top: topAreaBackground.top
        anchors.topMargin: 5
        anchors.left: topAreaBackground.left
        anchors.leftMargin: 5
        anchors.right: topAreaBackground.right
        anchors.rightMargin: 5
        anchors.bottom: topAreaBackground.bottom
        anchors.bottomMargin: 5

        BorderImage {
            anchors.fill: searchBoxBackground
            anchors { leftMargin: -1; topMargin: -1; rightMargin: -1; bottomMargin: -1 }
            //border { left: 10; top: 10; right: 10; bottom: 10 }
            source: "images/textInputBackground.svg"
            smooth: true
        }

        Rectangle {
            id: searchBoxBackground
            height: 22
            width: 70
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left

            TextInput {
                id: searchBox
                objectName: "searchBox"
                width: parent.width

                signal newResourceFilterText(string regexpText)
                onTextChanged: searchBox.newResourceFilterText(text)

                autoScroll: true
                selectByMouse: true
                font.pointSize: 10
                anchors.centerIn: parent
            }
        }

    }

    // This is the component that displays each resource in the list
    Component {
        id: resourceDelegate

        // The box that holds each resource item
        Rectangle {

            color: "#FFE2E2E2"
            height: 36
            anchors.left: parent.left
            anchors.right: parent.right

            border.width: 1
            border.color: "#FFEFEFEF"

            // Name and Type and Resource Icon
            Item {
                id: resourceId

                anchors.left: parent.left
                anchors.right: quantityControls.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.leftMargin: 8
                anchors.rightMargin: 8

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

            // Resource Quantity
            Item {
                id: quantityControls
                width: 80

                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                BorderImage {
                    anchors.fill: quantityBackground
                    anchors { leftMargin: -1; topMargin: -1; rightMargin: -1; bottomMargin: -1 }
                    border { left: 10; top: 10; right: 10; bottom: 10 }
                    source: "images/textInputBackground.svg"
                    smooth: true
                }

                Rectangle {
                    id: quantityBackground
                    //color: desiredQuantity.acceptableInput ? "green" : "red"
                    height: 14
                    width: 60

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: rightButton.left
                    anchors.leftMargin: 8

                    TextInput {
                        id: desiredQuantity
                        width: 55
                        autoScroll: true

                        text: quantity
                        selectByMouse: true

                        // Only allow integers, and set the text to
                        // be invalid if there is nothing in the box.
                        // NOTE: Just for fun, really.
                        validator: RegExpValidator{ regExp: /\d+/ }

                        font.pointSize: 10
                        anchors.centerIn: parent.Center
                    }
                }

                // Circle button control
                Rectangle {
                    id: rightButton

                    height: 14
                    width: 14
                    radius: width*0.5

                    // color: "transparent"
                    color: desiredQuantity.acceptableInput ? "green" : "red"
                    border.color: "black"
                    border.width: 2

                    smooth: true

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: width

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true
                        onEntered: {
                            parent.color = "gray"
                        }
                        onExited: {
                            parent.color = "transparent"
                        }
                    }
                }
            }


        }
    }

    Flickable{
        anchors.top: topArea.bottom
        anchors.topMargin: 5

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: lowerStatusBar.top

        clip: true

        ListView {
            id: resourceView
            anchors.fill: parent
            anchors.rightMargin: 20

            model: resourceModel
            delegate: resourceDelegate
        }
        ScrollBar {
            id: resourceScrollBar
            target: resourceView
        }
    }


    Item {
        id: lowerStatusBar

        height: 30
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            objectName: "statusBarBackground"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "lightgray"
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 8 + 5
            anchors.verticalCenter: parent.verticalCenter

            spacing: 10

            Text {
                text: qsTr("Here is the bottom bar.")
            }
        }
    }
}
