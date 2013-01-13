/*
        ScrollBar component for QML Flickable

        Copyright (c) 2010 Gregory Schlomoff - gregory.schlomoff@gmail.com

        This code is released under the MIT license

        Permission is hereby granted, free of charge, to any person obtaining a copy
        of this software and associated documentation files (the "Software"), to deal
        in the Software without restriction, including without limitation the rights
        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
        copies of the Software, and to permit persons to whom the Software is
        furnished to do so, subject to the following conditions:

        The above copyright notice and this permission notice shall be included in
        all copies or substantial portions of the Software.

        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
        THE SOFTWARE.
*/

/*
        Usage:

        Flickable {
          id: myFlickable
          ...
        }
        ScrollBar {
          target: myFlickable
        }
*/

import QtQuick 2.0

BorderImage {
    property variant target

    source: "images/scrollbar.svg"

    anchors {
        top: target.top;
        bottom: target.bottom;
        right: target.right;
        rightMargin: -width;
    }
    opacity: (track.height == slider.height) ? 0.40 : 1.00

    Item {
        anchors {fill: parent; margins: 0}

        Image {
            id: upArrow
            source: "images/up-arrow.svg"
            anchors.top: parent.top
            MouseArea {
                id: upArrowMouse
                anchors.fill: parent
                onPressed: {
                    timer.scrollAmount = -10
                    timer.running = true;
                }
                onReleased: {
                    timer.running = false;
                }
            }
            states: State {
                name: "mouse-down"
                when: upArrowMouse.pressed
                PropertyChanges {
                    target: upArrow
                    source: "images/up-arrow_click.svg"
                }
            }
        }

        Timer {
                property int scrollAmount

                id: timer
                repeat: true
                interval: 20
                onTriggered: {
                    target.contentY = Math.max(
                                    0, Math.min(
                                    target.contentY + scrollAmount,
                                    target.contentHeight - target.height));
                }
        }

        Item {
            id: track
            anchors {top: upArrow.bottom; topMargin: 0; bottom: downArrow.top;}
            width: parent.width

            MouseArea {
                anchors.fill: parent
                onPressed: {
                    // scroll by a page, until the scroll bar reaches the cursor
                    // TODO: This doesn't work.
                    if (slider.y > mouseY
                            || slider.y+slider.height < mouseY )
                    {
                        timer.scrollAmount = target.height * (mouseY < slider.y ? -1 : 1)
                        timer.running = true;
                        console.log("sliding")
                    }
                    else
                    {
                        timer.running = false
                    }
                }
                onReleased: {
                    timer.running = false;
                }
            }

            BorderImage {
                id:slider

                source: "images/slider.svg"
                width: parent.width

                height: target.visibleArea.heightRatio * track.height
                y: target.visibleArea.yPosition * track.height

                MouseArea {
                    anchors.fill: parent
                    drag.target: parent
                    drag.axis: Drag.YAxis
                    drag.minimumY: 0
                    drag.maximumY: track.height - height

                    onPositionChanged: {
                        if (pressedButtons == Qt.LeftButton) {
                             target.contentY = slider.y * target.contentHeight / track.height
                        }
                    }
                }
            }
        }
        Image {
            id: downArrow
            source: "images/dn-arrow.svg"
            anchors.bottom: parent.bottom
            MouseArea {
                id: downArrowMouse
                anchors.fill: parent
                onPressed: {
                    timer.scrollAmount = 10
                    timer.running = true;
                }
                onReleased: {
                    timer.running = false;
                }
            }
            states: State {
                name: "mouse-down"
                when: downArrowMouse.pressed
                PropertyChanges {
                    target: downArrow
                    source: "images/dn-arrow_click.svg"
                }
            }
        }
    }
}
