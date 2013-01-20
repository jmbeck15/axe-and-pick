import QtQuick 2.0

Rectangle {
    property string icon
    property variant target

    // The width and color properties should be
    // customized by the caller. The image in the
    // button is always padded by six pixels on
    // either size, and shrunk if necessary.
    width: imageIcon.sourceSize.width + 12
    color: "#FFeeeeee"

    // A toolbar button fills the height of the target object.
    anchors.top: target.top
    anchors.bottom: target.bottom

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
