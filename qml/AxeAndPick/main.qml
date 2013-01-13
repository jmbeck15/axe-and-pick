import QtQuick 2.0

Item {
    width: 800
    height: 600
    id: rootWindow

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
                id: openFileButton
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

        ResourceDelegate {
            id: resourceDelegate
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

    OpenFileDisplay {
        visible: true
        windowWidth: 300
        filePath: settings.value("TimberAndStone/GameInstallationDirectory");

        MouseArea {
            // This is to disable clicks going through the window.
            anchors.fill: parent
        }
    }
}


