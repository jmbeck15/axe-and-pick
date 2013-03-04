import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    id: openFileDialog

    property int windowWidth
    property string filePath
    property variant settingsObject

    anchors.fill: parent

    onVisibleChanged: {
        resetValidityMarker();
    }

    Rectangle {
        anchors.fill: parent
        color: "#80000000"
    }

    MouseArea {
        // This is to disable clicks going through the window.
        anchors.fill: parent
        onWheel: {} // do nothing
        onClicked: {
            openFileDialog.enabled = false;
            openFileDialog.visible = false;
        }
    }

    function resetValidityMarker()
    {
        directoryTextOutline.color = savesAccess.pathIsValid() ? "green" : "red";
        if( savesAccess.pathIsValid() )
        {
            savesAccess.loadGamesList();
        }
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

            height: 59
            color: "lightgray"

            Text {
                id: directoryTextTag
                text: "Timber and Stone 'saves.sav' file"
                color: "#FF303030"
                font.pixelSize: 11
                font.italic: true
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.left: parent.left
                anchors.leftMargin: 10
            }

            Rectangle {
                id: directoryTextOutline
                color: "gray"
                height: 22

                anchors.left: directoryTextTag.left
                anchors.right: openFileButton.left
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 10

                Rectangle {
                    id: directoryTextBackground
                    color: "#FFf8f8f8"
                    height: parent.height-2

                    clip: true
                    anchors.left: parent.left
                    anchors.leftMargin: 1
                    anchors.right: parent.right
                    anchors.rightMargin: 1
                    anchors.verticalCenter: parent.verticalCenter

                    TextInput {
                        id: directoryText
                        width: directoryTextBackground.width-12
                        anchors {
                            left: parent.left;
                            right: parent.right;
                            centerIn: parent;
                        }

                        text: savesAccess.getSavesPath();
                        onTextChanged: {
                            settings.setValue("TimberAndStone/GameInstallationDirectory",
                                              text);
                            savesAccess.setFilePath(text);

                            resetValidityMarker();
                        }

                        autoScroll: true
                        selectByMouse: true
                        font.pointSize: 10
                    }
                }
            }

            ToolbarImageButton {
                id: openFileButton
                target: directoryTextOutline
                icon: "images/openIcon.svg"
                color: "transparent"
                width: 40
                anchors.right: parent.right
                anchors.rightMargin: 6

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { parent.color = "#0A000000" }
                    onExited: { parent.color = "transparent" }
                    onClicked: {
                        // Open the file dialog
                        savesAccess.openFileDialog();
                        directoryText.text = savesAccess.getSavesPath();
                    }
                }
            }
        }

        Rectangle {
            id: listBorder
            anchors.top: savesFileDirectoryBackground.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: 3
            color: "silver"
        }

        SavedGameDelegate {
            id: savedGameDelegate
        }

        ListView {
            id: savedGameList

            anchors.top: listBorder.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 16

            model: savedGameModel
            delegate: savedGameDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        ScrollBar {
            target: savedGameList
            width: 16
        }

    }
}
