import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property int windowWidth

    anchors.fill: parent

    Rectangle {
        anchors.fill: parent
        color: "#80000000"
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
        border.color: "darkgray"
    }

    Item {
        id: fileOpenContents
        anchors.fill: fileOpenWindowBackground


    }
}
