import QtQuick 2.0

Rectangle {
    property string icon
    property string iconComplete
    property variant target

    function showClickAnimation() {
        clickAnimationOpacity.enabled = false;
        imageIconOperationComplete.opacity = 0.5;
        clickAnimationOpacity.enabled = true;
        imageIconOperationComplete.opacity = 0.0;
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
        width: parent.width - 16
        height: parent.height

        anchors.centerIn: parent
    }
    Image {
        id: imageIconOperationComplete
        source: iconComplete
        fillMode: Image.PreserveAspectFit
        width: parent.width - 16
        height: parent.height
        opacity: 0.0

        anchors.centerIn: parent

        Behavior on opacity {
            id: clickAnimationOpacity

            PropertyAnimation {
                duration: 3000
                easing.type: Easing.InExpo
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
