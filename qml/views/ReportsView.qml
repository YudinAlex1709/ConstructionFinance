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
                text: qsTr("Отчеты")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            Row {
                spacing: 8
                
                TextField {
                    id: fromDateField
                    placeholderText: qsTr("С даты")
                    text: new Date(new Date().getFullYear(), new Date().getMonth(), 1).toLocaleDateString(Qt.locale("ru_RU"), "dd.MM.yyyy")
                }
                
                TextField {
                    id: toDateField
                    placeholderText: qsTr("По дату")
                    text: new Date().toLocaleDateString(Qt.locale("ru_RU"), "dd.MM.yyyy")
                }
            }
        }
        
        // Report types grid
        GridLayout {
            Layout.fillWidth: true
            columns: Math.min(3, Math.floor(parent.width / 300))
            columnSpacing: 16
            rowSpacing: 16
            
            Repeater {
                model: [
                    { type: ReportManager.ProfitLoss, title: qsTr("P&L"), subtitle: qsTr("Прибыли и убытки"), icon: "\ue6e1" },
                    { type: ReportManager.CashFlow, title: qsTr("ДДС"), subtitle: qsTr("Движение денежных средств"), icon: "\ue84f" },
                    { type: ReportManager.BudgetVsActual, title: qsTr("Бюджет"), subtitle: qsTr("Бюджет vs Факт"), icon: "\ue8e5" },
                    { type: ReportManager.AccountsReceivable, title: qsTr("Дебиторка"), subtitle: qsTr("Дебиторская задолженность"), icon: "\ue8a1" },
                    { type: ReportManager.AccountsPayable, title: qsTr("Кредиторка"), subtitle: qsTr("Кредиторская задолженность"), icon: "\ue8a7" },
                    { type: ReportManager.ProjectSummary, title: qsTr("Проекты"), subtitle: qsTr("Сводка по проектам"), icon: "\ue2c7" }
                ]
                
                delegate: CFCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 120
                    highlighted: false
                    
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            var fromDate = new Date(fromDateField.text.split('.').reverse().join('-'))
                            var toDate = new Date(toDateField.text.split('.').reverse().join('-'))
                            var report = ReportManager.generateReport(modelData.type, fromDate, toDate)
                            reportPreview.reportData = report
                            reportPreview.open()
                        }
                    }
                    
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        spacing: 16
                        
                        Rectangle {
                            Layout.preferredWidth: 56
                            Layout.preferredHeight: 56
                            radius: 12
                            color: Material.accent
                            opacity: 0.15
                            
                            Label {
                                anchors.centerIn: parent
                                text: modelData.icon
                                font.family: "Material Icons"
                                font.pixelSize: 28
                                color: Material.accent
                            }
                        }
                        
                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 4
                            
                            Label {
                                text: modelData.title
                                font.pixelSize: 18
                                font.bold: true
                            }
                            Label {
                                text: modelData.subtitle
                                font.pixelSize: 13
                                color: Material.color(Material.Grey)
                            }
                        }
                        
                        Label {
                            text: "\ue5cc"
                            font.family: "Material Icons"
                            font.pixelSize: 24
                            color: Material.color(Material.Grey)
                        }
                    }
                }
            }
        }
    }
    
    // Report preview dialog
    Dialog {
        id: reportPreview
        title: qsTr("Предпросмотр отчета")
        modal: true
        anchors.centerIn: parent
        width: 600
        height: 500
        standardButtons: Dialog.Close
        
        property var reportData: null
        
        ScrollView {
            anchors.fill: parent
            clip: true
            
            ColumnLayout {
                width: reportPreview.width - 40
                spacing: 16
                
                Label {
                    visible: reportPreview.reportData !== null
                    text: reportPreview.reportData ? reportPreview.reportData.title : ""
                    font.pixelSize: 20
                    font.bold: true
                }
                
                Label {
                    visible: reportPreview.reportData !== null
                    text: reportPreview.reportData ? reportPreview.reportData.period : ""
                    font.pixelSize: 12
                    color: Material.color(Material.Grey)
                }
                
                GridLayout {
                    visible: reportPreview.reportData !== null
                    columns: 2
                    columnSpacing: 32
                    rowSpacing: 16
                    
                    Repeater {
                        model: reportPreview.reportData ? Object.keys(reportPreview.reportData).filter(function(k) {
                            return ['income', 'expense', 'profit', 'totalBudget', 'totalSpent', 'totalRemaining'].indexOf(k) !== -1
                        }) : []
                        
                        delegate: RowLayout {
                            Label {
                                text: {
                                    var labels = {
                                        'income': qsTr('Доходы'),
                                        'expense': qsTr('Расходы'),
                                        'profit': qsTr('Прибыль'),
                                        'totalBudget': qsTr('Общий бюджет'),
                                        'totalSpent': qsTr('Потрачено'),
                                        'totalRemaining': qsTr('Остаток')
                                    }
                                    return labels[modelData] || modelData
                                }
                                font.pixelSize: 13
                                color: Material.color(Material.Grey)
                            }
                            Label {
                                text: reportPreview.reportData[modelData].toLocaleString(Qt.locale("ru_RU"), "f", 0) + " ₽"
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                }
                
                Row {
                    spacing: 8
                    
                    CFButton {
                        text: qsTr("Экспорт PDF")
                        icon: "\ue415"
                        onClicked: {
                            // Export to PDF
                        }
                    }
                    
                    CFButton {
                        text: qsTr("Экспорт Excel")
                        icon: "\ue873"
                        outlined: true
                        onClicked: {
                            // Export to Excel
                        }
                    }
                }
            }
        }
    }
}
