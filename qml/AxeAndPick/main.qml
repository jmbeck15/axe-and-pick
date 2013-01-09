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

        Image {
            id: searchIcon
            source: "images/searchIcon.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 5
        }
        Rectangle {
            id: searchBoxOutline
            color: "#FF999999"
            height: 22
            width: 150
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: searchIcon.right
            anchors.leftMargin: 3

            Rectangle {
                id: searchBoxBackground
                color: "white"
                height: parent.height-2

                clip: true
                anchors.left: parent.left
                anchors.leftMargin: 1
                anchors.right: cancelSearchIcon.left
                anchors.verticalCenter: parent.verticalCenter

                TextInput {
                    id: searchBox
                    objectName: "searchBox"
                    width: searchBoxBackground.width-12
                    anchors.centerIn: parent
                    anchors {
                        left: parent.left;
                        right: parent.right;
                        leftMargin: 8;
                    }

                    // NOTE: This is how you use a signal, though it's
                    // totally not necessary in this case.
                    //signal newResourceFilterText(string regexpText)
                    //onTextChanged: searchBox.newResourceFilterText(text)
                    onTextChanged: resourceModelProxy.setFilterFixedString(text)

                    autoScroll: true
                    selectByMouse: true
                    font.pointSize: 10

                    focus: true
                }
            }
            Image {
                id: cancelSearchIcon
                source: "images/cancelSearch.svg"
                anchors.verticalCenter: parent.verticalCenter
                anchors.right: parent.right
                anchors.rightMargin: 1

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        searchBox.text = "";
                    }
                }
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
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter

                    Image {
                        id: subtractQuantity
                        source: "images/subtract.svg"
                        anchors.left: parent.left
                        anchors.leftMargin: 1
                        anchors.verticalCenter: parent.verticalCenter

                        MouseArea {
                            anchors.fill: parent

                            onClicked: {
                                if (mouse.modifiers & Qt.ShiftModifier)
                                {
                                    resourceModel.setData(identification, quantity-100)
                                }
                                else
                                {
                                    resourceModel.setData(identification, quantity-10)
                                }
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
                            anchors.centerIn: parent
                            anchors {
                                left: parent.left;
                                right: parent.right;
                                leftMargin: 8;
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
                            anchors.fill: parent
                            onClicked: {
                                if (mouse.modifiers & Qt.ShiftModifier)
                                {
                                    resourceModel.setData(identification, quantity+100)
                                }
                                else
                                {
                                    resourceModel.setData(identification, quantity+10)
                                }
                            }
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

            model: resourceModelProxy
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
