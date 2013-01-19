import QtQuick 2.0

Rectangle {
    property string icon
    property int setWidth

    setWidth: imageIcon.sourceSize.width + 12
    width: setWidth
    color: "#FFeeeeee"

    // A toolbar button fills the height of the parent.
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    // Button Icon
    Image {
        id: imageIcon
        source: icon
        fillMode: Image.PreserveAspectFit
        width: parent.width - 12
        height: parent.height

        anchors.centerIn: parent
    }
}
