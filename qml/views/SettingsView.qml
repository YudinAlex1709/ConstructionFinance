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
        Label {
            text: qsTr("Настройки")
            font.pixelSize: 28
            font.bold: true
        }
        
        // Settings sections
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 24
            
            // Left column - General settings
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16
                
                CFCard {
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Label {
                            text: qsTr("Общие настройки")
                            font.pixelSize: 18
                            font.bold: true
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Язык")
                                Layout.fillWidth: true
                            }
                            
                            ComboBox {
                                model: ["Русский", "English"]
                                currentIndex: Settings.language === "ru" ? 0 : 1
                                onCurrentIndexChanged: {
                                    Settings.language = currentIndex === 0 ? "ru" : "en"
                                }
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Тема")
                                Layout.fillWidth: true
                            }
                            
                            ComboBox {
                                model: [qsTr("Светлая"), qsTr("Темная")]
                                currentIndex: Settings.theme === "dark" ? 1 : 0
                                onCurrentIndexChanged: {
                                    Settings.theme = currentIndex === 1 ? "dark" : "light"
                                }
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Валюта по умолчанию")
                                Layout.fillWidth: true
                            }
                            
                            ComboBox {
                                model: ["RUB (₽)", "USD ($)", "EUR (€)"]
                                currentIndex: 0
                            }
                        }
                    }
                }
                
                CFCard {
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Label {
                            text: qsTr("Резервное копирование")
                            font.pixelSize: 18
                            font.bold: true
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Автоматическое резервное копирование")
                                Layout.fillWidth: true
                            }
                            
                            Switch {
                                checked: Settings.autoBackup
                                onCheckedChanged: Settings.autoBackup = checked
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Интервал (дней)")
                                Layout.fillWidth: true
                            }
                            
                            SpinBox {
                                value: Settings.backupInterval
                                from: 1
                                to: 30
                                onValueModified: Settings.backupInterval = value
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            spacing: 8
                            
                            CFButton {
                                text: qsTr("Создать копию")
                                onClicked: {
                                    // Create backup
                                }
                            }
                            
                            CFButton {
                                text: qsTr("Восстановить")
                                outlined: true
                                onClicked: {
                                    // Restore backup
                                }
                            }
                        }
                    }
                }
            }
            
            // Right column - Database and system
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 16
                
                CFCard {
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Label {
                            text: qsTr("База данных")
                            font.pixelSize: 18
                            font.bold: true
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Путь к базе данных")
                                Layout.fillWidth: true
                            }
                            
                            Label {
                                text: App.getDatabasePath()
                                font.pixelSize: 12
                                color: Material.color(Material.Grey)
                                elide: Text.ElideMiddle
                                Layout.maximumWidth: 200
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Размер базы данных")
                                Layout.fillWidth: true
                            }
                            
                            Label {
                                text: "2.5 MB"
                                font.pixelSize: 12
                                color: Material.color(Material.Grey)
                            }
                        }
                        
                        CFButton {
                            text: qsTr("Оптимизировать")
                            outlined: true
                            onClicked: {
                                // Optimize database
                            }
                        }
                    }
                }
                
                CFCard {
                    Layout.fillWidth: true
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Label {
                            text: qsTr("О приложении")
                            font.pixelSize: 18
                            font.bold: true
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Версия")
                                Layout.fillWidth: true
                            }
                            
                            Label {
                                text: App.version
                                font.pixelSize: 12
                                color: Material.color(Material.Grey)
                            }
                        }
                        
                        RowLayout {
                            Layout.fillWidth: true
                            
                            Label {
                                text: qsTr("Режим сборки")
                                Layout.fillWidth: true
                            }
                            
                            Label {
                                text: App.isDebugBuild ? "Debug" : "Release"
                                font.pixelSize: 12
                                color: Material.color(Material.Grey)
                            }
                        }
                        
                        Label {
                            text: "Construction Finance - система управления финансами строительных компаний"
                            font.pixelSize: 12
                            color: Material.color(Material.Grey)
                            wrapMode: Text.Wrap
                            Layout.fillWidth: true
                        }
                    }
                }
            }
        }
    }
}
