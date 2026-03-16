import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Pane {
    id: root
    
    property bool highlighted: false
    property alias content: contentContainer.children
    
    Material.elevation: highlighted ? 4 : 1
    padding: 0
    
    background: Rectangle {
        color: Material.background
        radius: 8
        border.color: highlighted ? Material.accent : Material.color(Material.Grey, Material.Shade300)
        border.width: highlighted ? 2 : 1
    }
    
    Item {
        id: contentContainer
        anchors.fill: parent
        anchors.margins: root.padding
    }
}
