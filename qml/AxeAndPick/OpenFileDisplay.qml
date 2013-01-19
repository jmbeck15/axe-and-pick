import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property int windowWidth
    property string filePath

    anchors.fill: parent

//    Rectangle {
//        anchors.fill: parent
//        color: "#80000000"
//    }

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

            height: 40
            color: "darkgray"

            Rectangle {
                id: directoryOutline
                color: "gray"
                height: 22
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 5
                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.right: openFileButton.left
                anchors.rightMargin: 5

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

                        text: filePath

                        onTextChanged: {
                            filePath = text
                        }
                        autoScroll: true
                        selectByMouse: true
                        font.pointSize: 10

                        focus: true
                    }
                }
            }
            ToolbarImageButton {
                id: openFileButton
                icon: "images/openIcon.svg"
                color: "transparent"
                setWidth: 40
                anchors.right: parent.right
                anchors.rightMargin: 5
            }
        }
    }
}
