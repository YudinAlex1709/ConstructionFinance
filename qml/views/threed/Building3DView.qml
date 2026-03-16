import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick3D
import ConstructionFinance 1.0

Page {
    id: root
    
    property var project: null
    
    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 10
            
            ToolButton {
                text: "\ue5c4"
                font.family: "Material Icons"
                onClicked: stackView.pop()
            }
            
            Label {
                text: project ? project.name + " - 3D модель" : qsTr("3D просмотр")
                font.pixelSize: 18
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            ComboBox {
                id: viewModeCombo
                model: [qsTr("Каркас"), qsTr("Сплошной"), qsTr("Текстурированный")]
                currentIndex: 1
                onCurrentIndexChanged: scene3D.setViewMode(currentIndex)
            }
            
            Button {
                text: qsTr("Тепловая карта")
                checkable: true
                onCheckedChanged: {
                    if (checked) scene3D.showFinancialHeatmap(getStageCosts())
                    else scene3D.clearHeatmap()
                }
            }
            
            Button {
                text: qsTr("По размеру")
                onClicked: scene3D.zoomToFit()
            }
        }
    }
    
    RowLayout {
        anchors.fill: parent
        
        // 3D Viewport
        Item {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            Scene3D {
                id: scene3D
                anchors.fill: parent
                
                Component.onCompleted: {
                    // Create a sample building
                    createProceduralBuilding(5, 20, 20, 3)
                }
            }
            
            // Overlay with info
            Rectangle {
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.margins: 10
                width: 250
                height: infoColumn.height + 20
                color: Material.background
                border.color: Material.color(Material.Grey, Material.Shade300)
                radius: 8
                opacity: 0.95
                
                Column {
                    id: infoColumn
                    anchors.centerIn: parent
                    spacing: 8
                    width: parent.width - 20
                    
                    Label {
                        text: qsTr("Финансовая сводка")
                        font.bold: true
                        font.pixelSize: 14
                    }
                    
                    Rectangle {
                        width: parent.width
                        height: 1
                        color: Material.color(Material.Grey, Material.Shade300)
                    }
                    
                    Row {
                        spacing: 10
                        Label { text: qsTr("Бюджет:"); width: 80 }
                        Label { 
                            text: project ? project.formattedBudget : "-"
                            font.bold: true
                        }
                    }
                    
                    Row {
                        spacing: 10
                        Label { text: qsTr("Потрачено:"); width: 80 }
                        Label { 
                            text: project ? project.formattedSpent : "-"
                            color: "#e74c3c"
                        }
                    }
                    
                    Row {
                        spacing: 10
                        Label { text: qsTr("Остаток:"); width: 80 }
                        Label { 
                            text: project ? project.formattedRemaining : "-"
                            color: "#27ae60"
                        }
                    }
                }
            }
        }
        
        // Stages panel
        Rectangle {
            Layout.preferredWidth: 300
            Layout.fillHeight: true
            color: Material.color(Material.Grey, Material.Shade50)
            border.color: Material.color(Material.Grey, Material.Shade200)
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10
                
                Label {
                    text: qsTr("Этапы строительства")
                    font.bold: true
                    font.pixelSize: 16
                }
                
                ListView {
                    id: stagesList
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 8
                    
                    model: [
                        { name: "Фундамент", budget: 5000000, spent: 4800000 },
                        { name: "Этаж 1", budget: 8000000, spent: 6500000 },
                        { name: "Этаж 2", budget: 8000000, spent: 4200000 },
                        { name: "Этаж 3", budget: 8000000, spent: 1500000 },
                        { name: "Этаж 4", budget: 8000000, spent: 0 },
                        { name: "Этаж 5", budget: 8000000, spent: 0 },
                        { name: "Крыша", budget: 3000000, spent: 0 }
                    ]
                    
                    delegate: CFCard {
                        width: stagesList.width
                        highlighted: false
                        
                        Column {
                            spacing: 4
                            width: parent.width - 32
                            
                            Label { 
                                text: modelData.name 
                                font.bold: true
                            }
                            ProgressBar {
                                width: parent.width
                                value: modelData.budget > 0 ? modelData.spent / modelData.budget : 0
                                Material.accent: value > 1 ? Material.Red : (value > 0.8 ? Material.Orange : Material.Green)
                            }
                            Row {
                                spacing: 10
                                Label { 
                                    text: qsTr("Бюджет: %1").arg(modelData.budget.toLocaleString(Qt.locale("ru_RU"), "f", 0)) + " ₽"
                                    font.pixelSize: 12
                                }
                                Label { 
                                    text: qsTr("Потрачено: %1").arg(modelData.spent.toLocaleString(Qt.locale("ru_RU"), "f", 0)) + " ₽"
                                    font.pixelSize: 12
                                    color: "#e74c3c"
                                }
                            }
                        }
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                scene3D.highlightStage(modelData.name)
                            }
                        }
                    }
                }
            }
        }
    }
    
    function getStageCosts() {
        var costs = {}
        for (var i = 0; i < stagesList.model.length; i++) {
            var stage = stagesList.model[i]
            costs[stage.name] = {
                budget: stage.budget,
                spent: stage.spent
            }
        }
        return costs
    }
}
