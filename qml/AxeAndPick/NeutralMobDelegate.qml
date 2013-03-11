import QtQuick 2.0

// This is the component that displays each human in the list
Component {

    Rectangle {

        color: "#FFeeeeee"
        height: 36
        anchors.left: parent.left
        anchors.right: parent.right

        border.width: 1
        border.color: "#FFE3E3E3"

        // Name and Type and Icon
        Item {
            id: mobID

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.leftMargin: 10

            // Name and Profession
            Item
            {
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter
                anchors.leftMargin: 8
                height: childrenRect.height

                Text {
                    id: typeText
                    text: type

                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right

                    font.pointSize: 10
                    clip: true
                }
                Text {
                    id: positionText
                    text: "x:" + posX + ", y:" + posY + ", z:" + posZ

                    anchors.top: typeText.bottom
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
