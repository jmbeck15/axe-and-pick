import QtQuick 2.0

Rectangle {
    property string icon

    width: imageIcon.width + 12
    color: "#FFeeeeee"

    // A toolbar button fills the height of the parent.
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    // Button Icon
    Image {
        id: imageIcon
        source: icon
        anchors.centerIn: parent
    }
}
