import QtQuick 2.0

// This is the component that displays each resource in the list
Component {


    // The box that holds each resource item
    Rectangle {

        color: "#FFeeeeee"
        height: 72
        anchors.left: parent.left
        anchors.right: parent.right

        border.width: 1
        border.color: "#FFe3e3e3"

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            onEntered: { parent.color = "#FFe5e5e5" }
            onExited: { parent.color = "#FFeeeeee" }
            onPressed: { parent.color = "#FFe0e0e0" }
            onClicked: {
                // Clear errors
                errorBar.errorsHaveOccured = false;

                // Load and display the game.
                savesAccess.loadSavedGame( name );

                // Set the game data
                saveGameName.text = model.name;
                saveGameDate.text = model.date;
                //dayInputButton.value = model.day;

                // Close the dialog
                openFileDialog.enabled = false;
                openFileDialog.visible = false;
            }
        }

        // Name and Date
        Item {
            id: nameAndDate

            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height

            Text {
                id: nameText
                text: model.name

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter

                font.pointSize: 14

                clip: true
            }
            Text {
                id: dateText
                text: model.date

                anchors.top: nameText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter

                font.italic: true
                font.pointSize: 7
                color: "grey"
                clip: true
            }
        }

        Rectangle {
            id: gameStatsBox
            color: "transparent"

            width: childrenRect.width
            anchors.top: nameAndDate.bottom
            anchors.topMargin: 2
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 7
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                id: worldSizeText
                color: "#FFe0e0e0"

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                width: childrenRect.width + 12

                border.width: 1
                border.color: "#FFc0c0c0"

                Text {
                    //anchors.centerIn: parent
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - font.pixelSize)/2 - 2

                    //anchors.horizontalCenter: parent.horizontalCenter
                    text: model.worldSize
                    font.pointSize: 10
                    color: "#FF565656"
                }
            }
            Rectangle {
                id: dayCountContainer
                color: "#FFe0e0e0"

                anchors.top: parent.top
                anchors.left: worldSizeText.right
                anchors.leftMargin: -1
                anchors.bottom: parent.bottom
                width: childrenRect.width + 12

                border.width: 1
                border.color: "#FFc0c0c0"

                Image {
                    id: dayCountIcon
                    source: "images/dayCountIcon.svg"
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - height)/2
                }
                Text {
                    id: dayCountText
                    //anchors.centerIn: parent
                    anchors.left: dayCountIcon.right
                    anchors.leftMargin: 4
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - font.pixelSize)/2 - 2

                    text: model.day
                    font.pointSize: 10
                    color: "#FF565656"
                }
            }
            Rectangle {
                id: unitNumberContainer
                color: "#FFe0e0e0"

                anchors.top: parent.top
                anchors.left: dayCountContainer.right
                anchors.leftMargin: -1
                anchors.bottom: parent.bottom
                width: childrenRect.width + 12

                border.width: 1
                border.color: "#FFc0c0c0"

                Image {
                    id: unitNumberIcon
                    source: "images/unitNumberIcon.svg"
                    anchors.left: parent.left
                    anchors.leftMargin: 6
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - height)/2
                }
                Text {
                    id: unitNumberText
                    anchors.left: unitNumberIcon.right
                    anchors.leftMargin: 4
                    anchors.top: parent.top
                    anchors.topMargin: (parent.height - font.pixelSize)/2 - 2

                    text: model.unitNumber
                    font.pointSize: 10
                    color: "#FF565656"
                }
            }
        }

    }
}
