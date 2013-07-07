import QtQuick 2.0

Component {

    Rectangle {

        function getTypeDescription(type, subtype) {
            switch (type) {
            case "Skeleton":
                switch (subtype) {
                case 0:
                    return "Skeleton"
                case 2:
                    return "Bloodthirsty Skeleton"
                case 3:
                    return "Mace Skeleton"
                default:
                    return "Unknown Skeleton"
                }
            case "Goblin":
                switch (subtype) {
                case 0:
                    return "Goblin Ravager"
                case 1:
                    return "Goblin Marauder"
                case 2:
                    return "Goblin Archer"
                case 6:
                    return "Mounted Goblin"
                default:
                    return "Unknown Goblin"
                }
            case "Wolf":
                switch (subtype) {
                case 0:
                    return "Wolf (Type 0)"
                case 1:
                    return "Wolf (Type 1)"
                case 2:
                    return "Wolf (Type 2)"
                default:
                    return "Unknown Wolf"
                }
            case "Spider":
                switch (subtype) {
                case 0:
                    return "Spider"
                case 1:
                    return "Larger Spider"
                case 2:
                    return "Spider Matriach"
                default:
                    return "Unknown Spider"
                }
            case "Necromancer":
                switch (subtype) {
                case 0:
                    return "Necromancer"
                default:
                    return "Unknown Necromancer"
                }
            default:
                return "Unknown Unit"
            }
        }

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

            source: "images/units/" + type + subtype + ".svg"
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
                text: getTypeDescription(type,subtype)

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
                    violentMobModel.remove(id)
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
