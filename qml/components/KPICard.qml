import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

Card {
    id: root
    
    property string title
    property real value
    property string change
    property bool changePositive: true
    property string subtitle
    property string icon
    property color cardColor
    
    Layout.fillWidth: true
    Layout.preferredHeight: 120
    
    RowLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16
        
        Rectangle {
            Layout.preferredWidth: 56
            Layout.preferredHeight: 56
            radius: 12
            color: Qt.rgba(root.cardColor.r, root.cardColor.g, root.cardColor.b, 0.15)
            
            Label {
                anchors.centerIn: parent
                text: root.icon
                font.family: "Material Icons"
                font.pixelSize: 28
                color: root.cardColor
            }
        }
        
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4
            
            Label {
                text: root.title
                font.pixelSize: 13
                color: Material.color(Material.Grey)
            }
            
            Label {
                text: {
                    if (root.value >= 1000000000)
                        return (root.value / 1000000000).toFixed(1) + "B ₽"
                    else if (root.value >= 1000000)
                        return (root.value / 1000000).toFixed(1) + "M ₽"
                    else if (root.value >= 1000)
                        return (root.value / 1000).toFixed(1) + "K ₽"
                    else
                        return root.value.toLocaleString(Qt.locale("ru_RU"), "f", 0) + " ₽"
                }
                font.pixelSize: 24
                font.bold: true
            }
            
            Row {
                spacing: 8
                visible: root.change !== ""
                
                Label {
                    text: root.change
                    font.pixelSize: 12
                    color: root.changePositive ? "#4CAF50" : "#F44336"
                }
                
                Label {
                    text: root.subtitle
                    font.pixelSize: 12
                    color: Material.color(Material.Grey)
                    visible: root.subtitle !== ""
                }
            }
        }
    }
}
