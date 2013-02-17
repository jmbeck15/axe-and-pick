import QtQuick 2.0

Rectangle {
    property int value
    property string textLabel
    property bool disabled

    disabled: false
    width: 34
    color: "#FFeeeeee"

    // A toolbar button fills the height of the parent.
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    Rectangle {
        id: numberBoxOutline
        color: "gray"
        height: 22

        anchors.top: parent.top
        anchors.topMargin: 6
        anchors.left: parent.left
        anchors.leftMargin: 3
        anchors.right: parent.right
        anchors.rightMargin: 3

        Rectangle {
            color: disabled ? "lightgray" : "white"
            height: parent.height-2
            width: parent.width-2
            clip: true

            anchors.centerIn: parent

            TextInput {
                id: numberInput
                width: parent.width-12
                anchors {
                    left: parent.left;
                    right: parent.right;
                    leftMargin: 2;
                    centerIn: parent;
                }
                readOnly: disabled ? true : false

                text: value
                color: numberInput.acceptableInput ? "black" : "red"
                horizontalAlignment: Text.AlignHCenter

                // Only allow integers, and set the text to
                // be invalid if there is nothing in the box.
                validator: RegExpValidator{ regExp: /\d+/ }

                autoScroll: true
                selectByMouse: true
                font.pixelSize: 16
                onTextChanged: {
                    //resourceModel.setData(identification, parseInt(text,10))
                }
            }
        }
    }

    Text {
        text: textLabel
        color: "gray"
        font.pixelSize: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
    }

}
