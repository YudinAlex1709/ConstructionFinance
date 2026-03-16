import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtCharts
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
                text: qsTr("Финансы")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            CFButton {
                text: qsTr("Новая операция")
                icon: "\ue145"
                highlighted: true
                onClicked: newTransactionDialog.open()
            }
        }
        
        // Summary cards
        GridLayout {
            Layout.fillWidth: true
            columns: 3
            columnSpacing: 16
            rowSpacing: 16
            
            CFCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    
                    Label {
                        text: qsTr("Доходы за месяц")
                        font.pixelSize: 13
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        text: FinanceManager.monthlyIncome.toLocaleString(Qt.locale("ru_RU"), "f", 0) + " ₽"
                        font.pixelSize: 24
                        font.bold: true
                        color: "#4CAF50"
                    }
                }
            }
            
            CFCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    
                    Label {
                        text: qsTr("Расходы за месяц")
                        font.pixelSize: 13
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        text: FinanceManager.monthlyExpense.toLocaleString(Qt.locale("ru_RU"), "f", 0) + " ₽"
                        font.pixelSize: 24
                        font.bold: true
                        color: "#F44336"
                    }
                }
            }
            
            CFCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 100
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    
                    Label {
                        text: qsTr("Чистая прибыль")
                        font.pixelSize: 13
                        color: Material.color(Material.Grey)
                    }
                    Label {
                        text: (FinanceManager.monthlyIncome - FinanceManager.monthlyExpense).toLocaleString(Qt.locale("ru_RU"), "f", 0) + " ₽"
                        font.pixelSize: 24
                        font.bold: true
                        color: (FinanceManager.monthlyIncome - FinanceManager.monthlyExpense) >= 0 ? "#4CAF50" : "#F44336"
                    }
                }
            }
        }
        
        // Charts
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 24
            
            CFCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    
                    Label {
                        text: qsTr("Доходы и расходы по месяцам")
                        font.pixelSize: 16
                        font.bold: true
                    }
                    
                    ChartView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        antialiasing: true
                        legend.visible: true
                        legend.alignment: Qt.AlignBottom
                        animationOptions: ChartView.SeriesAnimations
                        
                        BarSeries {
                            name: qsTr("Доходы")
                            color: "#4CAF50"
                            BarSet { label: qsTr("Янв"); values: [450000] }
                            BarSet { label: qsTr("Фев"); values: [520000] }
                            BarSet { label: qsTr("Мар"); values: [480000] }
                            BarSet { label: qsTr("Апр"); values: [610000] }
                            BarSet { label: qsTr("Май"); values: [580000] }
                            BarSet { label: qsTr("Июн"); values: [650000] }
                        }
                        
                        BarSeries {
                            name: qsTr("Расходы")
                            color: "#F44336"
                            BarSet { label: qsTr("Янв"); values: [320000] }
                            BarSet { label: qsTr("Фев"); values: [380000] }
                            BarSet { label: qsTr("Мар"); values: [350000] }
                            BarSet { label: qsTr("Апр"); values: [420000] }
                            BarSet { label: qsTr("Май"); values: [400000] }
                            BarSet { label: qsTr("Июн"); values: [450000] }
                        }
                    }
                }
            }
            
            CFCard {
                Layout.preferredWidth: 400
                Layout.fillHeight: true
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    
                    Label {
                        text: qsTr("Расходы по категориям")
                        font.pixelSize: 16
                        font.bold: true
                    }
                    
                    ChartView {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        antialiasing: true
                        legend.visible: true
                        legend.alignment: Qt.AlignRight
                        
                        PieSeries {
                            PieSlice { label: qsTr("Материалы"); value: 45; color: "#FF6384" }
                            PieSlice { label: qsTr("Зарплата"); value: 30; color: "#36A2EB" }
                            PieSlice { label: qsTr("Аренда"); value: 15; color: "#FFCE56" }
                            PieSlice { label: qsTr("Прочее"); value: 10; color: "#4BC0C0" }
                        }
                    }
                }
            }
        }
    }
}
