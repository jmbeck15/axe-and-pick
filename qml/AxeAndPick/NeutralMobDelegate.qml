import QtQuick 2.0

Component {

    Rectangle {

        color: "#FFeeeeee"
        height: 26
        anchors.left: parent.left
        anchors.right: parent.right

        border.width: 1
        border.color: "#FFE3E3E3"

        Image {
            id: typeIcon
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            source: "images/units/" + type + ".svg"
            fillMode: Image.PreserveAspectFit
        }

        Item {
            id: mobDetails

            anchors.left: typeIcon.right
            anchors.leftMargin: 5
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom

            // Name of the mob
            Text {
                id: typeText
                text: type

                anchors.verticalCenter: parent.verticalCenter
                font.pointSize: 10
                clip: true
            }
        }

        Image {
            id: deleteButton
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter
            source: "images/deleteButton.svg"

            MouseArea {
                id: deleteButtonArea
                anchors.fill: parent
                onClicked: {
                    neutralMobModel.remove(id)
                }
            }
            states:
                State { // Pressed
                    when: deleteButtonArea.pressed
                    PropertyChanges {
                        target: deleteButton
                        source: "images/deleteButtonPressed.svg"
                    }
                }
        }
    }
}
