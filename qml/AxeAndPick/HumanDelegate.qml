import QtQuick 2.0
//import Human 1.0

// This is the component that displays each human in the list
Component {

    Rectangle {

        color: "#FFeeeeee"
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right

        border.width: 1
        border.color: "#FFE3E3E3"

        // Name and Type and Human Icon
        Item {
            id: humanID

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10

            // Name and Profession
            Item
            {
                id: nameAndProfession
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 8
                height: childrenRect.height

                Text {
                    id: nameText
                    text: name + " miner:" + minerLevel + " stoneMason:" + stoneMasonLevel

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 10
                    clip: true
                }
                Text {
                    id: typeProfession
                    text: profession + " " + posX + " " + posY

                    anchors.top: nameText.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 7
                    color: "grey"
                    clip: true
                }
            }
        }
    }
}
