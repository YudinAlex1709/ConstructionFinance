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
                text: qsTr("Контрагенты")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            TextField {
                id: searchField
                placeholderText: qsTr("Поиск контрагентов...")
                Layout.preferredWidth: 300
            }
            
            CFButton {
                text: qsTr("Новый контрагент")
                icon: "\ue145"
                highlighted: true
                onClicked: contractorDialog.open()
            }
        }
        
        // Filter tabs
        TabBar {
            id: filterTabs
            Layout.fillWidth: true
            
            TabButton { text: qsTr("Все") }
            TabButton { text: qsTr("Заказчики") }
            TabButton { text: qsTr("Поставщики") }
            TabButton { text: qsTr("Субподрядчики") }
            TabButton { text: qsTr("Сотрудники") }
        }
        
        // Contractors list
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
                        Layout.preferredWidth: 200
                        text: qsTr("Название")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 120
                        text: qsTr("Тип")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.preferredWidth: 150
                        text: qsTr("ИНН")
                        font.bold: true
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        Layout.fillWidth: true
                        text: qsTr("Контакты")
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
                    
                    // TODO: Replace with actual model
                    model: [
                        { name: "ООО СтройИнвест", type: 0, typeLabel: "Заказчик", inn: "7701234567", phone: "+7 (495) 123-45-67", email: "info@stroyinvest.ru" },
                        { name: "ООО Материалы", type: 1, typeLabel: "Поставщик", inn: "7707654321", phone: "+7 (495) 765-43-21", email: "sales@materials.ru" },
                        { name: "ИП Иванов", type: 2, typeLabel: "Субподрядчик", inn: "770000000001", phone: "+7 (916) 123-45-67", email: "ivanov@mail.ru" }
                    ]
                    
                    delegate: Rectangle {
                        width: parent.width
                        height: 60
                        color: index % 2 === 0 ? "transparent" : Material.color(Material.Grey, Material.Shade50)
                        
                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 8
                            
                            Label {
                                Layout.preferredWidth: 200
                                text: modelData.name
                                font.pixelSize: 14
                                font.bold: true
                                elide: Text.ElideRight
                            }
                            
                            Rectangle {
                                Layout.preferredWidth: 100
                                Layout.preferredHeight: 24
                                radius: 12
                                color: {
                                    switch(modelData.type) {
                                        case 0: return "#E3F2FD"
                                        case 1: return "#E8F5E9"
                                        case 2: return "#FFF3E0"
                                        case 3: return "#F3E5F5"
                                        default: return "#ECEFF1"
                                    }
                                }
                                
                                Label {
                                    anchors.centerIn: parent
                                    text: modelData.typeLabel
                                    font.pixelSize: 11
                                    color: {
                                        switch(modelData.type) {
                                            case 0: return "#1565C0"
                                            case 1: return "#2E7D32"
                                            case 2: return "#E65100"
                                            case 3: return "#7B1FA2"
                                            default: return "#455A64"
                                        }
                                    }
                                }
                            }
                            
                            Label {
                                Layout.preferredWidth: 150
                                text: modelData.inn
                                font.pixelSize: 13
                                font.family: "monospace"
                            }
                            
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 2
                                
                                Label {
                                    text: modelData.phone
                                    font.pixelSize: 12
                                }
                                Label {
                                    text: modelData.email
                                    font.pixelSize: 12
                                    color: Material.color(Material.Grey)
                                }
                            }
                            
                            Row {
                                Layout.preferredWidth: 100
                                
                                ToolButton {
                                    text: "\ue3c9"
                                    font.family: "Material Icons"
                                    font.pixelSize: 18
                                    onClicked: contractorDialog.open()
                                }
                                
                                ToolButton {
                                    text: "\ue92b"
                                    font.family: "Material Icons"
                                    font.pixelSize: 18
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    // Contractor dialog
    ContractorDialog {
        id: contractorDialog
    }
}
