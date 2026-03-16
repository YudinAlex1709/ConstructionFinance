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
                text: qsTr("Операции")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            TextField {
                id: searchField
                placeholderText: qsTr("Поиск операций...")
                Layout.preferredWidth: 300
            }
            
            CFButton {
                text: qsTr("Новая операция")
                icon: "\ue145"
                highlighted: true
                onClicked: newTransactionDialog.open()
            }
        }
        
        // Filter tabs
        TabBar {
            id: filterTabs
            Layout.fillWidth: true
            
            TabButton { text: qsTr("Все") }
            TabButton { text: qsTr("Доходы") }
            TabButton { text: qsTr("Расходы") }
            TabButton { text: qsTr("Переводы") }
        }
        
        // Transactions table
        CFCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                
                // Table header
                RowLayout {
                    Layout.fillWidth: true
                    height: 40
                    
                    Label {
                        Layout.preferredWidth: 120
                        text: qsTr("Дата")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 150
                        text: qsTr("Тип")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Описание")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 150
                        text: qsTr("Проект")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 150
                        text: qsTr("Сумма")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 100
                        text: qsTr("Статус")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 100
                        text: qsTr("Действия")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                }
                
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: Material.color(Material.Grey, Material.Shade300)
                }
                
                // Table content
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 0
                    model: FinanceManager.getTransactions(100)
                    
                    delegate: Rectangle {
                        width: parent.width
                        height: 50
                        color: index % 2 === 0 ? "transparent" : Material.color(Material.Grey, Material.Shade50)
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            
                            Label {
                                Layout.preferredWidth: 120
                                text: modelData.formattedDate
                                font.pixelSize: 13
                            }
                            
                            Rectangle {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 24
                                radius: 12
                                color: {
                                    if (modelData.isIncome) return "#E8F5E9"
                                    if (modelData.isExpense) return "#FFEBEE"
                                    return "#E3F2FD"
                                }
                                
                                Label {
                                    anchors.centerIn: parent
                                    text: modelData.typeLabel
                                    font.pixelSize: 11
                                    color: {
                                        if (modelData.isIncome) return "#2E7D32"
                                        if (modelData.isExpense) return "#C62828"
                                        return "#1565C0"
                                    }
                                }
                            }
                            
                            Label {
                                Layout.fillWidth: true
                                text: modelData.description || modelData.category || "-"
                                font.pixelSize: 13
                                elide: Text.ElideRight
                            }
                            
                            Label {
                                Layout.preferredWidth: 150
                                text: modelData.projectName || "-"
                                font.pixelSize: 13
                                elide: Text.ElideRight
                            }
                            
                            Label {
                                Layout.preferredWidth: 150
                                text: modelData.formattedAmount
                                font.pixelSize: 13
                                font.bold: true
                                color: modelData.isIncome ? "#4CAF50" : (modelData.isExpense ? "#F44336" : Material.foreground)
                            }
                            
                            Rectangle {
                                Layout.preferredWidth: 80
                                Layout.preferredHeight: 24
                                radius: 12
                                color: modelData.status === 1 ? "#E8F5E9" : "#FFF3E0"
                                
                                Label {
                                    anchors.centerIn: parent
                                    text: modelData.statusLabel
                                    font.pixelSize: 11
                                    color: modelData.status === 1 ? "#2E7D32" : "#E65100"
                                }
                            }
                            
                            Row {
                                Layout.preferredWidth: 100
                                
                                ToolButton {
                                    text: "\ue3c9"
                                    font.family: "Material Icons"
                                    font.pixelSize: 18
                                    onClicked: {
                                        // Edit transaction
                                    }
                                }
                                
                                ToolButton {
                                    text: "\ue92b"
                                    font.family: "Material Icons"
                                    font.pixelSize: 18
                                    onClicked: {
                                        FinanceManager.deleteTransaction(modelData.id)
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
