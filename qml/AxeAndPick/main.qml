import QtQuick 2.0

Rectangle {
    width: 360
    height: 460
    color: "#FFEFEFEF"
    id: iAmTheRootObject

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 40
        color: "lightgray"
    }

    Item {
        id: topArea
        height: 40

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

        BorderImage {
            anchors.fill: searchBoxBackground
            anchors { leftMargin: -1; topMargin: -1; rightMargin: -1; bottomMargin: -1 }
            border { left: 10; top: 10; right: 10; bottom: 10 }
            source: "images/textInputBackground.svg"
            smooth: true
        }

        Rectangle {
            id: searchBoxBackground
            height: 14
            width: 60

            anchors.verticalCenter: parent.verticalCenter
            anchors.right: rightButton.left
            anchors.leftMargin: 8

            TextInput {
                id: searchBox

                text: searchRegex

                width: 60
                autoScroll: true
                selectByMouse: true
                font.pointSize: 10
                anchors.verticalCenter: parent.verticalCenter
            }
        }

    }

    // This is the component that displays each resource in the list
    Component {
        id: resourceDelegate

        // The box that holds each game content
        Rectangle {

            color: "#FFE2E2E2"
            height: 36
            anchors.left: parent.left
            anchors.right: parent.right

            border.width: 1
            border.color: "#FFEFEFEF"


            // Resource Icon and Text
            Item {
                id: resourceId

                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter

                anchors.leftMargin: 8
                anchors.right: quantityControls.left


                Image {
                    id: resourceIcon
                    source: "qrc:///" + icon
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 3
                }
                Text {
                    text: name

                    anchors.left: resourceIcon.right
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 5
                    anchors.right: parent.right

                    font.family: "Helvetica"
                    font.pointSize: 10

                    clip: true
                }
            }

            // Resource Quantity
            Item {
                id: quantityControls

                width: 120
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
                    width: 55

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: rightButton.left
                    anchors.leftMargin: 8

                    TextInput {
                        id: desiredQuantity
                        width: 50
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

        anchors.left: topArea.left
        anchors.right: topArea.right

        anchors.bottom: lowerStatusBar.top

        clip: true

        ListView {
            id: resourceView
            anchors.fill: parent
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
