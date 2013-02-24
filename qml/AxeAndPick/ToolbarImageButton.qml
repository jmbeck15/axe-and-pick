import QtQuick 2.0

Rectangle {
    property string icon
    property variant target

    function showClickAnimation()
    {
        clickAnimationScale.enabled = false;
        imageIcon.scale = 1.30;
        clickAnimationScale.enabled = true;
        imageIcon.scale = 1.00;
    }

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

        Behavior on scale {
            id: clickAnimationScale

            PropertyAnimation {
                duration: 500
                easing.type: Easing.OutExpo
            }
        }
    }
    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onEntered: { parent.color = "#0A000000" }
        onExited: { parent.color = "transparent" }
    }
}
