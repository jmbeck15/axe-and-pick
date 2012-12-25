import QtQuick 2.0

Rectangle {
    width: 360
    height: 460
    color: "#FFEFEFEF"
    id: iAmTheRootObject

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: 40
        color: "lightgray"
    }

    Item {
        id: topArea
        height: 40

        anchors.top: parent.top
        anchors.topMargin: 5
        anchors.left: parent.left
        anchors.leftMargin: 5
        anchors.right: parent.right
        anchors.rightMargin: 5

    }

    // This is the component that displays each game in the list
    Component {
        id: gameDelegate

        // The box that holds each game content
        Rectangle {

            color: "#FFE2E2E2"
            height: 36
            anchors.left: parent.left
            anchors.right: parent.right

            border.width: 4
            border.color: "#FFEFEFEF"


            // Players
            Text{
                text: player1 + " <b>vs</b> " + player2

                font.family: "Helvetica"
                font.pointSize: 10

                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 8
                anchors.right: statsAndControls.left

                clip: true
            }

            // Game Stats and Controls
            Item {
                id: statsAndControls

                width: 120
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom

                Text{
                    text: "Markers: " + markers

                    font.pointSize: 7

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 8
                }

                // Circle button control
                Rectangle {

                    height: 14
                    width: 14
                    radius: width*0.5

                    color: "transparent"
                    border.color: "black"
                    border.width: 2

                    smooth: true

                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: width

                    MouseArea {
                        anchors.fill: parent

                        hoverEnabled: true
                        onEntered: {
                            parent.color = "gray"
                        }
                        onExited: {
                            parent.color = "transparent"
                        }
                    }
                }
            }


        }
    }

    Flickable{
        anchors.top: topArea.bottom
        anchors.topMargin: 5

        anchors.left: topArea.left
        anchors.right: topArea.right

        anchors.bottom: lowerStatusBar.top

        clip: true

//        ListView {
//            anchors.fill: parent
//            model: gameList
//            delegate: gameDelegate
//        }
    }

    Item {
        id: lowerStatusBar

        height: 30
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            objectName: "statusBarBackground"
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            color: "lightgray"
        }

        Row {
            anchors.left: parent.left
            anchors.leftMargin: 8 + 5
            anchors.verticalCenter: parent.verticalCenter

            spacing: 10

            Text {
                text: qsTr("Here is the bottom bar.")
            }
        }
    }
}
