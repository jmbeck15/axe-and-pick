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

                Rectangle {
                    color: desiredQuantity.acceptableInput ? "green" : "red"
                    height: 14
                    width: desiredQuantity.width + 10

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8

                    TextInput {
                        id: desiredQuantity

                        text: quantity
                        selectByMouse: true

                        // Only allow integers
                        validator: RegExpValidator{ regExp: /\d+/ }

                        font.pointSize: 10
                        anchors.centerIn: parent
                    }
                }

                // Circle button control
                Rectangle {


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
