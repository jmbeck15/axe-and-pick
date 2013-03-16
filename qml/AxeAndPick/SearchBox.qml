import QtQuick 2.0



Item {
    // This is the target for the regular expression.
    property variant target

    width: searchIcon.width + searchBoxOutline.width + searchBoxOutline.anchors.leftMargin

    function clear()
    {
        // Force clearing the text and updating the model.
        searchBox.text = ""
        target.model.setFilterRegExp("^(?!unknown).*")
    }

    Image {
        id: searchIcon
        source: "images/searchIcon.svg"
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
    }
    Rectangle {
        id: searchBoxOutline
        color: "gray"
        height: 22
        width: 150
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: searchIcon.right
        anchors.leftMargin: 5

        Rectangle {
            id: searchBoxBackground
            color: "#FFf8f8f8"
            height: parent.height-2

            clip: true
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.right: cancelSearchIcon.left
            anchors.verticalCenter: parent.verticalCenter

            TextInput {
                id: searchBox
                objectName: "searchBox"
                width: searchBoxBackground.width-12
                anchors {
                    left: parent.left;
                    right: parent.right;
                    centerIn: parent;
                }

                onTextChanged: target.model.setFilterRegExp("^(?!unknown).*" + text)

                autoScroll: true
                selectByMouse: true
                font.pointSize: 10

                focus: true
            }
        }
        Image {
            id: cancelSearchIcon
            source: "images/cancelSearch.svg"
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: parent.right
            anchors.rightMargin: 1

            MouseArea {
                id: iconClick
                anchors.fill: parent
                onClicked: {
                    searchBox.text = "";
                }
            }
            states: State {
                name: "mouse-down"
                when: iconClick.pressed
                PropertyChanges {
                    target: cancelSearchIcon
                    source: "images/cancelSearch_click.svg"
                }
            }
        }
    }
}
