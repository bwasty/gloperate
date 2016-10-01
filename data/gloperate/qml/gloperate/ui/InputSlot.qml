
import QtQuick 2.0

import gloperate.base 1.0
import gloperate.ui 1.0


/**
*  InputSlot
*
*  Representation of an input slot
*/
BaseItem
{
    id: slot

    // Called when mouse cursor has entered the slot
    signal entered()

    // Called when mouse cursor has left the slot
    signal exited()

    // Called when mouse button has been pressed on the slot
    signal pressed()

    // Options
    property string path:        ''         ///< Path in the pipeline hierarchy (e.g., 'pipeline.stage1.input1')
    property alias  name:        label.text ///< Name of the slot
    property bool   hovered:     false      ///< Is the slot currently hovered?
    property bool   selected:    false      ///< Is the slot currently selected?
    property bool   connectable: true       ///< Enable interaction for connecting to another slot?
    property int    radius:      Ui.style.pipelineConnectorSize
    property color  color:       Ui.style.pipelineConnectorColorIn
    property color  borderColor: Ui.style.pipelineLineColor
    property int    borderWidth: Ui.style.pipelineLineWidth

    implicitWidth:  row.implicitWidth  + 2 * row.anchors.margins
    implicitHeight: row.implicitHeight + 2 * row.anchors.margins

    // Slot container
    Row
    {
        id: row

        anchors.left: parent.left
        anchors.top:  parent.top
        spacing:      Ui.style.paddingSmall

        // Connector
        Rectangle
        {
            id: connector

            anchors.verticalCenter: parent.verticalCenter
            width:                  slot.radius
            height:                 slot.radius

            radius:       width / 2.0
            color:        slot.selected ? Ui.style.pipelineColorSelected : (slot.hovered ? Ui.style.pipelineColorHighlighted : slot.color)
            border.color: slot.borderColor
            border.width: slot.borderWidth
        }

        // Name of slot
        Label
        {
            id: label

            anchors.verticalCenter: parent.verticalCenter

            text:  'Value'
            color: Ui.style.pipelineTextColor
        }
    }

    // Mouse area for selection
    MouseArea
    {
        anchors.fill: parent

        hoverEnabled: true

        onEntered: slot.entered();
        onExited:  slot.exited();
        onPressed: slot.pressed();
    }
}