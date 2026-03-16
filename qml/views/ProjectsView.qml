import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import ConstructionFinance 1.0

Page {
    id: root
    padding: 24
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 24
        
        // Header
        RowLayout {
            Layout.fillWidth: true
            
            Label {
                text: qsTr("Проекты")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            TextField {
                id: searchField
                placeholderText: qsTr("Поиск проектов...")
                Layout.preferredWidth: 300
                onTextChanged: ProjectManager.searchProjects(text)
            }
            
            CFButton {
                text: qsTr("Новый проект")
                icon: "\ue145"
                highlighted: true
                onClicked: newProjectDialog.open()
            }
        }
        
        // Filter tabs
        TabBar {
            id: filterTabs
            Layout.fillWidth: true
            
            TabButton { text: qsTr("Все (%1)").arg(ProjectManager.totalCount) }
            TabButton { text: qsTr("Активные (%1)").arg(ProjectManager.activeCount) }
            TabButton { text: qsTr("Планирование (%1)").arg(ProjectManager.planningCount) }
            TabButton { text: qsTr("Завершенные (%1)").arg(ProjectManager.completedCount) }
        }
        
        // Projects grid
        GridView {
            id: projectsGrid
            Layout.fillWidth: true
            Layout.fillHeight: true
            cellWidth: 350
            cellHeight: 220
            clip: true
            model: ProjectManager.projects
            
            delegate: CFCard {
                width: projectsGrid.cellWidth - 16
                height: projectsGrid.cellHeight - 16
                highlighted: false
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 12
                    
                    // Header
                    RowLayout {
                        Layout.fillWidth: true
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            
                            Label {
                                text: modelData.name
                                font.pixelSize: 16
                                font.bold: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                            Label {
                                text: modelData.code
                                font.pixelSize: 12
                                color: Material.color(Material.Grey)
                            }
                        }
                        
                        Rectangle {
                            Layout.preferredWidth: 100
                            Layout.preferredHeight: 24
                            radius: 12
                            color: {
                                switch(modelData.status) {
                                    case 0: return "#FFF3E0" // Planning - orange
                                    case 1: return "#E8F5E9" // Active - green
                                    case 2: return "#ECEFF1" // OnHold - grey
                                    case 3: return "#E3F2FD" // Completed - blue
                                    case 4: return "#FFEBEE" // Cancelled - red
                                    default: return "#ECEFF1"
                                }
                            }
                            
                            Label {
                                anchors.centerIn: parent
                                text: modelData.statusLabel
                                font.pixelSize: 11
                                color: {
                                    switch(modelData.status) {
                                        case 0: return "#E65100"
                                        case 1: return "#2E7D32"
                                        case 2: return "#455A64"
                                        case 3: return "#1565C0"
                                        case 4: return "#C62828"
                                        default: return "#455A64"
                                    }
                                }
                            }
                        }
                    }
                    
                    // Address
                    Label {
                        text: modelData.address || qsTr("Адрес не указан")
                        font.pixelSize: 12
                        color: Material.color(Material.Grey)
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                    }
                    
                    // Budget info
                    RowLayout {
                        Layout.fillWidth: true
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: qsTr("Бюджет")
                                font.pixelSize: 11
                                color: Material.color(Material.Grey)
                            }
                            Label {
                                text: modelData.formattedBudget
                                font.pixelSize: 14
                                font.bold: true
                            }
                        }
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: qsTr("Потрачено")
                                font.pixelSize: 11
                                color: Material.color(Material.Grey)
                            }
                            Label {
                                text: modelData.formattedSpent
                                font.pixelSize: 14
                                color: modelData.isOverBudget ? "#F44336" : Material.foreground
                            }
                        }
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            
                            Label {
                                text: qsTr("Остаток")
                                font.pixelSize: 11
                                color: Material.color(Material.Grey)
                            }
                            Label {
                                text: modelData.formattedRemaining
                                font.pixelSize: 14
                                color: modelData.budgetRemaining < 0 ? "#F44336" : "#4CAF50"
                            }
                        }
                    }
                    
                    // Progress
                    ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 4
                        
                        ProgressBar {
                            Layout.fillWidth: true
                            value: modelData.budget > 0 ? modelData.budgetSpent / modelData.budget : 0
                            Material.accent: value > 1 ? Material.Red : (value > 0.8 ? Material.Orange : Material.Green)
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Использование: %1%").arg(modelData.budgetUtilization.toFixed(1))
                                font.pixelSize: 11
                                color: Material.color(Material.Grey)
                            }
                            
                            Item { Layout.fillWidth: true }
                            
                            Label {
                                text: qsTr("Прогресс: %1%").arg(modelData.progress.toFixed(0))
                                font.pixelSize: 11
                                color: Material.color(Material.Grey)
                            }
                        }
                    }
                    
                    // Actions
                    RowLayout {
                        Layout.fillWidth: true
                        
                        Item { Layout.fillWidth: true }
                        
                        ToolButton {
                            text: "\ue3c9"
                            font.family: "Material Icons"
                            onClicked: {
                                // Edit project
                            }
                        }
                        
                        ToolButton {
                            text: "\ue80d"
                            font.family: "Material Icons"
                            onClicked: {
                                // View 3D
                                stackView.push("qrc:/qml/views/threed/Building3DView.qml", { project: modelData })
                            }
                        }
                        
                        ToolButton {
                            text: "\ue92b"
                            font.family: "Material Icons"
                            onClicked: {
                                // Delete project
                                confirmDeleteDialog.projectId = modelData.id
                                confirmDeleteDialog.projectName = modelData.name
                                confirmDeleteDialog.open()
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Confirm delete dialog
    Dialog {
        id: confirmDeleteDialog
        title: qsTr("Подтвердите удаление")
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Yes | Dialog.No
        
        property string projectId
        property string projectName
        
        Label {
            text: qsTr("Вы уверены, что хотите удалить проект \"%1\"?").arg(confirmDeleteDialog.projectName)
        }
        
        onAccepted: {
            ProjectManager.deleteProject(projectId)
        }
    }
}
