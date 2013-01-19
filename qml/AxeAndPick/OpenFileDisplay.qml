import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property int windowWidth
    property string filePath
    property variant settingsObject

    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#80000000"
    }

    MouseArea {
        // This is to disable clicks going through the window.
        anchors.fill: parent
        onWheel: {} // do nothing
        onClicked: { parent.visible = false }
    }

    Rectangle {
        id: fileOpenWindowBackground
        width: windowWidth
        color: "silver"

        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30

        // NOTE: The width is 5 to reflect consistancy with the draggable
        // border between lists.
        border.width: 5
        border.color: "gray"

        MouseArea {
            // This is to disable clicks going through the window.
            anchors.fill: parent
            onClicked: {}
        }
    }


    Item {
        id: fileOpenContents
        anchors.top: fileOpenWindowBackground.top
        anchors.topMargin: 5
        anchors.left: fileOpenWindowBackground.left
        anchors.leftMargin: 5
        anchors.right: fileOpenWindowBackground.right
        anchors.rightMargin: 5
        anchors.bottom: fileOpenWindowBackground.bottom
        anchors.bottomMargin: 5

        Rectangle {
            id: savesFileDirectoryBackground
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            height: 45
            color: "lightgray"

            Text {
                id: directoryTextTag
                text: "Game Installation Directory"
                color: "gray"
                font.pixelSize: 10
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
            }

            Rectangle {
                id: directoryOutline
                color: "gray"
                height: 22

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: openFileButton.left
                anchors.rightMargin: 5
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5

                Rectangle {
                    id: directoryBackground
                    color: "#FFf8f8f8"
                    height: parent.height-2

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    TextInput {
                        id: directoryBox
                        width: directoryBackground.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            centerIn: parent;
                        }

                        text: settings.value("TimberAndStone/GameInstallationDirectory");
                        onTextChanged: {
                            settings.setValue("TimberAndStone/GameInstallationDirectory",
                                              text);
                        }

                        autoScroll: true
                        selectByMouse: true
                        font.pointSize: 10
                    }
                }
            }
            ToolbarImageButton {
                id: openFileButton
                icon: "images/openIcon.svg"
                color: "transparent"
                width: 40
                anchors.right: parent.right
                anchors.rightMargin: 5

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { parent.color = "#0A000000" }
                    onExited: { parent.color = "transparent" }
                }
            }
        }

        SavedGamesDelegate {
            id: savedGamesDelegate
        }

        ListView {
            id: savedGamesList

            anchors.top: savesFileDirectoryBackground.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 16

            model: resourceModelProxy
            delegate: savedGamesDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        ScrollBar {
            target: savedGamesList
            width: 16
        }

    }
}
