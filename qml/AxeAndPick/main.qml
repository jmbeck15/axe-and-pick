import QtQuick 2.0

Item {
    width: 800
    height: 600
    id: rootWindow

    FontLoader {
        id: localNameFont
        name: "Arial"
        //source: "ITCEDSCR.TTF" // TODO: This doesn't work with OTF fonts.
    }

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
                id: dayInputButton
                textLabel: "Day"
                value: 0
                width: 45
                disabled: true
            }
//            ToolbarTextInput {
//                id: hourInputButton
//                textLabel: "Hour"
//                value: 0
//                width: 38
//                disabled: true
//            }
        }

        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom

            ToolbarImageButton {
                id: openFileButton
                target: parent
                icon: "images/openIcon.svg"
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { parent.color = "#0A000000" }
                    onExited: { parent.color = "transparent" }
                    onClicked: {
                        openFileDialog.enabled = true;
                        openFileDialog.visible = true;
                        // Unnecessary animation, right?
                        //openFileButton.showClickAnimation();
                    }
                }
            }
            ToolbarImageButton {
                id: saveSavedGamesButton
                target: parent
                icon: "images/saveIcon.svg"
                iconComplete: "images/saveIcon_green.svg"
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        savesAccess.saveSavedGame();
                        saveSavedGamesButton.showClickAnimation();
                    }
                }
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
            ToolbarImageButton {
                id: test0
                target: parent
                icon: "images/saveIcon.svg"
                color: "red"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        resourceList.model = resourceModelProxy2
                        resourceList.delegate = resourceDelegateThick
                        resourceSearchBox.clear()
                    }
                }
            }
            ToolbarImageButton {
                id: test1
                anchors.left: test0.right
                target: parent
                icon: "images/saveIcon.svg"
                color: "blue"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        resourceList.model = resourceModelProxy
                        resourceList.delegate = resourceDelegate
                        resourceSearchBox.clear()
                    }
                }
            }

            SearchBox {
                id: resourceSearchBox
                target: resourceList
                anchors.right: parent.right

                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ResourceDelegate {
            id: resourceDelegate
        }
        ResourceDelegateThick {
            id: resourceDelegateThick
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
        x: 320
        width: 5

        Rectangle {
            anchors.fill: parent
            color: "#ff949494"
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

        // Background for the unit container
        Rectangle {
            anchors.fill: parent
            color: "silver"
        }

        Item {
            id: unitToolbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30

            Rectangle {
                anchors.fill: parent
                color: "silver"
            }
        }

        HumanDelegate {
            id: humanDelegate
        }

        ListView {
            id: unitList
            anchors.top: unitToolbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.bottom: parent.bottom

            model: humanModelProxy
            delegate: humanDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        ScrollBar {
            target: unitList
            width: 16
        }
    }

    OpenFileDisplay {
        id: openFileDialog
        visible: true
        windowWidth: 300
        settingsObject: settings
    }
}


