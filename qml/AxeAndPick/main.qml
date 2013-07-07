import QtQuick 2.0

Item {
    width: 700
    height: 600
    id: rootWindow

    Item {
        id: mainToolbar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 50

        Rectangle {
            anchors.fill: parent
            color: "#FFeeeeee"
        }
        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.leftMargin: 10
            anchors.bottom: parent.bottom

            Column {
                spacing: 3
                anchors.top: parent.top
                anchors.topMargin: 5

                Text {
                    id: saveGameName
                    text: ""
                    font.pixelSize: 18
                }
                Text {
                    id: saveGameDate
                    text: ""
                    font.pixelSize: 10
                    font.italic: true
                    color: "gray"
                }
            }
            Rectangle {
                // This is just a spacer.
                color: "transparent"
                width: 30
                height: 1
            }
//            ToolbarTextInput {
//                id: dayInputButton
//                textLabel: "Day"
//                value: 0
//                width: 45
//                disabled: true
//            }
//            ToolbarTextInput {
//                id: hourInputButton
//                textLabel: "Hour"
//                value: 0
//                width: 38
//                disabled: true
//            }
        }

        Row {
            spacing: 5
            anchors.top: parent.top
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.bottom: parent.bottom

            // Utility for doing things.
            Image {
                id: utilityButton
                visible: false
                source: "images/utilityButton.svg"
                MouseArea {
                    id: utilityButtonArea
                    anchors.fill: parent
                    onClicked: {
                        savesAccess.writeToMatlab(8);
                    }
                }
                states:
                    State { // Pressed
                        when: utilityButtonArea.pressed
                        PropertyChanges {
                            target: utilityButton
                            source: "images/utilityButtonPressed.svg"
                        }
                    }
            }

            ToolbarImageButton {
                id: openFileButton
                target: parent
                icon: "images/openIcon.svg"
                color: "transparent"
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: { parent.color = "#0A000000" }
                    onExited: { parent.color = "transparent" }
                    onClicked: {
                        openFileDialog.enabled = true;
                        openFileDialog.visible = true;
                        // Unnecessary animation, right?
                        //openFileButton.showClickAnimation();
                    }
                }
            }
            ToolbarImageButton {
                id: saveSavedGamesButton
                target: parent
                icon: "images/saveIcon.svg"
                iconComplete: "images/saveIcon_green.svg"
                color: "transparent"

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        errorBar.errorsHaveOccured = false;
                        savesAccess.saveSavedGame();
                        saveSavedGamesButton.showClickAnimation();
                    }
                }

                Connections {
                    target: savesAccess
                    onFileLoadStatusChanged: {
                        if (errorOccured || errorBar.errorsHaveOccured) {
                            saveSavedGamesButton.icon = "images/saveIcon_disabled.svg";
                            saveSavedGamesButton.enabled = false;
                            console.debug("Error: " + message);
                        }
                        else {
                            saveSavedGamesButton.icon = "images/saveIcon.svg"
                            saveSavedGamesButton.enabled = true;
                            console.debug("Message: " + message);
                        }
                    }
                    onFileSaveStatusChanged: {
                        if (errorOccured || errorBar.errorsHaveOccured) {
                            saveSavedGamesButton.iconComplete = "images/saveIcon_red.svg";
                        }
                        else {
                            saveSavedGamesButton.iconComplete = "images/saveIcon_green.svg";
                        }
                    }
                }
            }
        }

    }

    Item {
        id: resourceContainer
        anchors.top: mainToolbar.bottom
        anchors.left: parent.left
        anchors.right: containerSeperator.left
        anchors.bottom: errorBar.top

        // Background for the resource container
        Rectangle {
            anchors.fill: parent
            color: "silver"
        }

        Item {
            id: resourceToolbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 26

            Rectangle {
                anchors.fill: parent
                color: "silver"
            }

            Image {
                id: resourceDiscoverButton
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                source: "images/resourceDiscoverButton.svg"
                MouseArea {
                    id: resourceDiscoverButtonArea
                    anchors.fill: parent
                    onClicked: {
                        resourceModel.setQuantitiesInOrder();
                    }
                }
                states:
                    State { // Pressed
                        when: resourceDiscoverButtonArea.pressed
                        PropertyChanges {
                            target: resourceDiscoverButton
                            source: "images/resourceDiscoverButtonPressed.svg"
                        }
                    }
            }

            SearchBox {
                id: resourceSearchBox
                target: resourceList
                anchors.right: parent.right

                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ResourceDelegate {
            id: resourceDelegate
        }

        ListView {
            id: resourceList
            anchors.top: resourceToolbar.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 16
            anchors.bottom: parent.bottom

            model: resourceModelProxy
            delegate: resourceDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        ScrollBar {
            target: resourceList
            width: 16
        }
    }

    Item {
        id: containerSeperator
        anchors.top: mainToolbar.bottom
        anchors.bottom: errorBar.top
        x: 320
        width: 5

        Rectangle {
            anchors.fill: parent
            color: "#ff949494"
        }
        MouseArea {
            anchors.fill: parent
            drag.target: containerSeperator

            // Restrict how far you can drag the seperator.
            drag.minimumX: 10
            drag.maximumX: rootWindow.width - 10

            cursorShape: Qt.SizeHorCursor
        }
    }

    Item {
        id: unitContainer
        anchors.top: mainToolbar.bottom
        anchors.left: containerSeperator.right
        anchors.right: parent.right
        anchors.bottom: errorBar.top

        // Background for the unit container
        Rectangle {
            anchors.fill: parent
            color: "silver"
        }

        Item {
            id: unitToolbar
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            height: 26

            Rectangle {
                anchors.fill: parent
                color: "silver"
            }

            // The buttons for selecting unit type
            Rectangle {
                id: unitButtonBox
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 10

                color: "gray"

                height: 22
                width: (32 * 3) + 2

                Image {
                    id: humanButton
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: "images/humanButton.svg"
                    property bool selected: true
                    MouseArea {
                        id: humanButtonArea
                        anchors.fill: parent
                        onClicked: {
                            humanButton.selected = true
                            neutralMobButton.selected = false
                            violentMobButton.selected = false
                            unitList.model = humanModelProxy
                            unitList.delegate = humanDelegate
                            addUnitGrid.model = humanTypeModel
                        }
                    }
                    states: [
                        State { // Pressed
                            when: humanButtonArea.pressed
                            PropertyChanges {
                                target: humanButton
                                source: "images/humanButtonPressed.svg"
                            }
                        },
                        State { // Selected
                            when: humanButton.selected
                            PropertyChanges {
                                target: humanButton
                                source: "images/humanButtonSelected.svg"
                            }
                        }]
                }
                Image {
                    id: neutralMobButton
                    anchors.centerIn: parent
                    source: "images/neutralMobButton.svg"
                    property bool selected
                    MouseArea {
                        id: neutralMobButtonArea
                        anchors.fill: parent
                        onClicked: {
                            humanButton.selected = false
                            neutralMobButton.selected = true
                            violentMobButton.selected = false
                            unitList.model = neutralMobModelProxy
                            unitList.delegate = neutralMobDelegate
                            addUnitGrid.model = neutralMobTypeModel
                        }
                    }
                    states: [
                        State { // Pressed
                            when: neutralMobButtonArea.pressed
                            PropertyChanges {
                                target: neutralMobButton
                                source: "images/neutralMobButtonPressed.svg"
                            }
                        },
                        State { // Selected
                            when: neutralMobButton.selected
                            PropertyChanges {
                                target: neutralMobButton
                                source: "images/neutralMobButtonSelected.svg"
                            }
                        }]
                }
                Image {
                    id: violentMobButton
                    anchors.right: parent.right
                    source: "images/violentMobButton.svg"
                    property bool selected
                    MouseArea {
                        id: violentMobButtonArea
                        anchors.fill: parent
                        onClicked: {
                            unitList.model = violentMobModelProxy
                            unitList.delegate = violentMobDelegate
                            humanButton.selected = false
                            neutralMobButton.selected = false
                            violentMobButton.selected = true
                            addUnitGrid.model = violentMobTypeModel
                        }
                    }
                    states: [
                        State { // Pressed
                            when: violentMobButtonArea.pressed
                            PropertyChanges {
                                target: violentMobButton
                                source: "images/violentMobButtonPressed.svg"
                            }
                        },
                        State { // Selected
                            when: violentMobButton.selected
                            PropertyChanges {
                                target: violentMobButton
                                source: "images/violentMobButtonSelected.svg"
                            }
                        }]
                }
            }

            // Button for adding units.
            Image {
                id: addButton
                anchors.left: unitButtonBox.right
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter
                source: "images/addButton.svg"
                property int selected
                MouseArea {
                    id: addButtonArea
                    anchors.fill: parent
                    onClicked: {
                        if (parent.selected) {
                            parent.selected = 0;
                        }
                        else {
                            parent.selected = 1;
                        }
                    }
                }
                states: [
                    State { // Pressed
                        when: addButtonArea.pressed
                        PropertyChanges {
                            target: addButton
                            source: "images/addButtonPressed.svg"
                        }
                    },
                    State { // Selected
                        when: addButton.selected
                        PropertyChanges {
                            target: addButton
                            source: "images/addButtonPressed.svg"
                        }
                    }
                ]
            }

            // Give all the units coffee.
            Image {
                id: serveCoffeeButton
                anchors.right: parent.right
                anchors.rightMargin: 10 + 16 // the 16 is for the scroll bar.
                anchors.verticalCenter: parent.verticalCenter
                source: "images/coffee.svg"
                MouseArea {
                    id: coffeeButtonArea
                    anchors.fill: parent
                    onClicked: {
                        humanModel.serveCoffee();
                    }
                }
                states:
                    State { // Pressed
                        when: coffeeButtonArea.pressed
                        PropertyChanges {
                            target: serveCoffeeButton
                            source: "images/coffeePressed.svg"
                        }
                    }
            }

        }

        // Container for things you can add to the list.
        Rectangle {
            id: addUnitContainer
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: unitToolbar.bottom

            // I'd break up this equation a bit, but I want all the variables
            // bound to this height property. This makes sure that all the
            // items fit in the grid. Toggle addButton.selected to zero the
            // height if you want to "close" the menu.
            height: addButton.selected *
                    (Math.ceil(addUnitGrid.count /
                     Math.floor( addUnitGrid.width / addUnitGrid.cellWidth )
                     )) * addUnitGrid.cellHeight + addButton.selected*8
            color: "gray"

            GridView {
                id: addUnitGrid
                model: humanTypeModel
                delegate: unitTypeDelegate

                anchors.left: parent.left
                anchors.leftMargin: 10
                // This width is being used instead of anchoring, because the
                // QtQuick2 GridView has a bug that doesn't wrap items properly.
                // The "16" in there is to act as a buffer so the items don't
                // hide under the edge of the window border.
                width: Math.floor((parent.width-16) / (cellWidth)) * cellWidth
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.bottom: parent.bottom

                cellWidth: 34
                cellHeight: 24

                clip: true
                interactive: false
            }

            // Animate height transitions
            Behavior on height {
                PropertyAnimation { easing.type: Easing.InOutQuad;
                                    duration: 150 }
            }
        }

        HumanDelegate {
            id: humanDelegate
        }
        NeutralMobDelegate {
            id: neutralMobDelegate
        }
        ViolentMobDelegate {
            id: violentMobDelegate
        }

        ListView {
            id: unitList
            anchors.top: addUnitContainer.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.rightMargin: 16

            anchors.bottom: parent.bottom

            model: humanModelProxy
            delegate: humanDelegate

            clip: true
            boundsBehavior: Flickable.StopAtBounds
        }

        ScrollBar {
            target: unitList
            width: 16
        }
    }

    Rectangle {
        id: errorBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // This needs to be cleared, or the last error won't go away.
        property bool errorsHaveOccured: false

        height: 0
        color: "gray"
        clip: true

        Rectangle {
            id: topBar
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
            height: 3
            color: "red"
        }

        Rectangle {
            anchors.top: topBar.bottom;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom

            Image {
                id: warningIcon
                anchors.left: parent.left
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter

                source: "images/warningIcon.svg"
                fillMode: Image.PreserveAspectFit
            }
            Text {
                id: errorMessage
                anchors.left: warningIcon.right
                anchors.leftMargin: 5
                anchors.right: parent.right
                anchors.verticalCenter: parent.verticalCenter

                text: "No error has occured."
                font.pointSize: 10

                color: "red"
            }
        }
        Connections {
            target: savesAccess
            onFileLoadStatusChanged: {
                if (errorOccured && !errorBar.errorsHaveOccured) {
                    errorMessage.text = message;
                    errorBar.errorsHaveOccured = true;

                    errorMessage.color = "red"
                    topBar.color = "red"
                    warningIcon.source = "images/warningIcon.svg"
                    errorBar.height = 29;
                } else
                if (errorBar.errorsHaveOccured) {
                    // Do nothing, because an error already occured.
                }
                else {
                    errorMessage.text = message;

                    errorMessage.color = "green"
                    topBar.color = "green"
                    warningIcon.source = "images/successIcon.svg"
                    errorBar.height = 0;
                }
            }
            onFileSaveStatusChanged: {
                if (errorOccured && !errorBar.errorsHaveOccured) {
                    errorMessage.text = message;
                    errorBar.errorsHaveOccured = true;

                    errorMessage.color = "red"
                    topBar.color = "red"
                    warningIcon.source = "images/warningIcon.svg"
                    errorBar.height = 29;
                }
                if (errorBar.errorsHaveOccured) {
                    // Do nothing, because an error already occured.
                }
                else {
                    errorMessage.text = message;

                    errorMessage.color = "green"
                    topBar.color = "green"
                    warningIcon.source = "images/successIcon.svg"
                    errorBar.height = 0;
                }
            }
        }
        // Animate height transitions
        Behavior on height {
            PropertyAnimation { easing.type: Easing.InOutQuad;
                                duration: 150 }
        }
    }


    OpenFileDisplay {
        id: openFileDialog
        visible: true
        windowWidth: 300
        settingsObject: settings
    }


    //
    // Unit Types
    //
    // The names reflect the <name>.svg image name too,
    // so make sure there is a valid image in the units/ folder.
    //
    ListModel {
        id: humanTypeModel
        ListElement {
            type: "Archer"
            subtype: ""
        }
        ListElement {
            type: "Blacksmith"
            subtype: ""
        }
        ListElement {
            type: "Builder"
            subtype: ""
        }
        ListElement {
            type: "Carpenter"
            subtype: ""
        }
        ListElement {
            type: "Engineer"
            subtype: ""
        }
        ListElement {
            type: "Farmer"
            subtype: ""
        }
        ListElement {
            type: "Fisherman"
            subtype: ""
        }
        ListElement {
            type: "Forager"
            subtype: ""
        }
        ListElement {
            type: "Infantry"
            subtype: ""
        }
        ListElement {
            type: "Miner"
            subtype: ""
        }
        ListElement {
            type: "Stone Mason"
            subtype: ""
        }
        ListElement {
            type: "Wood Chopper"
            subtype: ""
        }
        ListElement {
            type: "Tailor"
            subtype: ""
        }
        ListElement {
            type: "Trader"
            subtype: ""
        }
        ListElement {
            type: "Herder"
            subtype: ""
        }
        ListElement {
            type: "Adventurer"
            subtype: ""
        }
    }
    ListModel {
        id: neutralMobTypeModel
        ListElement {
            type: "Boar"
            subtype: ""
        }
        ListElement {
            type: "Chicken"
            subtype: ""
        }
        ListElement {
            type: "Sheep"
            subtype: ""
        }
    }
    ListModel {
        id: violentMobTypeModel
        ListElement {
            type: "Goblin"
            subtype: 0
        }
        ListElement {
            type: "Goblin"
            subtype: 1
        }
        ListElement {
            type: "Goblin"
            subtype: 2
        }
        ListElement {
            type: "Goblin"
            subtype: 6
        }
        ListElement {
            type: "Necromancer"
            subtype: 0
        }
        ListElement {
            type: "Skeleton"
            subtype: 0
        }
        // Skeleton subtype 1 seems to be replaced by
        // subtype 0 when loaded in-game.
        ListElement {
            type: "Skeleton"
            subtype: 2
        }
        ListElement {
            type: "Skeleton"
            subtype: 3
        }
        ListElement {
            type: "Spider"
            subtype: 0
        }
        ListElement {
            type: "Spider"
            subtype: 1
        }
        ListElement {
            type: "Spider"
            subtype: 2
        }
        ListElement {
            type: "Wolf"
            subtype: 0
        }
        ListElement {
            type: "Wolf"
            subtype: 1
        }
        ListElement {
            type: "Wolf"
            subtype: 2
        }
    }
    Component {
        id: unitTypeDelegate

        Rectangle {

            // First, we have to figure out where this unit should be placed.
            //
            // Ideally, this position should be taken from the map data, so we
            // know the location is valid (not in a tree, not under sand, etc).
            // But I don't have time to do that. Instead, this will simply
            // copy the x/y/z location of the first human already on the map.
            // If there are no humans, it takes the first neutral mob. If there
            // are no neutral mobs, it takes the first violent mob. I can't think
            // of any better solution. Placing the new units around the campfire
            // could fail depending on land geometry, and wouldn't work if the
            // campfire was removed. So for now, this is how it'll have to work.
            //
            // NOTE: I don't want the user changing the x/y/z location because
            // that's just annoying. The program should be able to figure this
            // stuff out automatically.
            //
            function addUnit() {

                if (humanButton.selected) {
                    if (humanModel.rowCount()) {
                        humanModel.add(type,humanModel.getFirstPosition(0),humanModel.getFirstPosition(1),humanModel.getFirstPosition(2));
                    } else if (neutralMobModel.rowCount()) {
                        humanModel.add(type,neutralMobModel.getFirstPosition(0),neutralMobModel.getFirstPosition(1),neutralMobModel.getFirstPosition(2));
                    } else if (violentMobModel.rowCount()) {
                        humanModel.add(type,violentMobModel.getFirstPosition(0),violentMobModel.getFirstPosition(1),violentMobModel.getFirstPosition(2));
                    }
                }
                else if (neutralMobButton.selected) {
                    if (neutralMobModel.rowCount()) {
                        neutralMobModel.add(type,neutralMobModel.getFirstPosition(0),neutralMobModel.getFirstPosition(1),neutralMobModel.getFirstPosition(2));
                    } else if (humanModel.rowCount()) {
                        neutralMobModel.add(type,humanModel.getFirstPosition(0),humanModel.getFirstPosition(1),humanModel.getFirstPosition(2));
                    } else if (violentMobModel.rowCount()) {
                        neutralMobModel.add(type,violentMobModel.getFirstPosition(0),violentMobModel.getFirstPosition(1),violentMobModel.getFirstPosition(2));
                    }
                }
                else if (violentMobButton.selected) {
                    if (violentMobModel.rowCount()) {
                        violentMobModel.add(type,subtype,violentMobModel.getFirstPosition(0),violentMobModel.getFirstPosition(1),violentMobModel.getFirstPosition(2));
                    } else if (neutralMobModel.rowCount()) {
                        violentMobModel.add(type,subtype,neutralMobModel.getFirstPosition(0),neutralMobModel.getFirstPosition(1),neutralMobModel.getFirstPosition(2));
                    } else if (humanModel.rowCount()) {
                        violentMobModel.add(type,subtype,humanModel.getFirstPosition(0),humanModel.getFirstPosition(1),humanModel.getFirstPosition(2));
                    }
                }
                else { console.log("No unit type buttons are enabled. Where do I add this unit?") }
            }

            Image {
                id: unitTypeIcon
                source:  "images/units/" + type + subtype + ".svg"

                MouseArea {
                    id: unitTypeIconArea
                    anchors.fill: parent
                    onClicked: {
                        addUnit();
                    }
                }
                states:
                    State { // Pressed
                        when: unitTypeIconArea.pressed
                        PropertyChanges {
                            target: plus
                            color: "green"
                            font.bold: true
                        }
                    }
            }
            Text {
                id: plus
                text: "+"
                color: "white"
                anchors.bottom: unitTypeIcon.bottom
            }
        }
    }
}


