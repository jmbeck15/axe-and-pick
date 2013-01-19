import QtQuick 2.0

// This is the component that displays each resource in the list
Component {


    // The box that holds each resource item
    Rectangle {

        color: "#FFeeeeee"
        height: 58
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
        }


        // Name and Date
        Item {
            id: nameAndDate

            anchors.top: parent.top
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.right: parent.right

            Text {
                id: nameText
                text: name

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter

                font.family: localNameFont.name
                font.pointSize: 12

                clip: true
            }
            Text {
                id: dateText
                text: "February 03 08:34 PM"

                anchors.top: nameText.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter

                font.family: localNameFont.name
                font.italic: true
                font.pointSize: 7
                color: "grey"
                clip: true
            }
        }

    }
}
