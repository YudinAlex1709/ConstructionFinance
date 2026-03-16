import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

Button {
    id: root
    
    property string icon: ""
    property bool outlined: false
    
    contentItem: Row {
        spacing: 8
        anchors.centerIn: parent
        
        Label {
            text: root.icon
            font.family: "Material Icons"
            font.pixelSize: 20
            visible: root.icon !== ""
            anchors.verticalCenter: parent.verticalCenter
            color: root.outlined ? Material.accent : "white"
        }
        
        Label {
            text: root.text
            font: root.font
            anchors.verticalCenter: parent.verticalCenter
            color: root.outlined ? Material.accent : "white"
        }
    }
    
    background: Rectangle {
        implicitWidth: 100
        implicitHeight: 40
        color: root.outlined ? "transparent" : (root.highlighted ? Material.accent : Material.primary)
        border.color: root.outlined ? Material.accent : "transparent"
        border.width: root.outlined ? 1 : 0
        radius: 4
        
        Behavior on color {
            ColorAnimation { duration: 100 }
        }
    }
}
