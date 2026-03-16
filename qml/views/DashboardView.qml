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
                text: qsTr("Финансовый обзор")
                font.pixelSize: 28
                font.bold: true
            }
            
            Item { Layout.fillWidth: true }
            
            ComboBox {
                id: periodCombo
                model: [qsTr("Сегодня"), qsTr("Эта неделя"), qsTr("Этот месяц"), qsTr("Этот квартал"), qsTr("Этот год")]
                currentIndex: 2
            }
        }
        
        // KPI Cards
        GridLayout {
            Layout.fillWidth: true
            columns: Math.min(4, Math.floor(parent.width / 280))
            columnSpacing: 16
            rowSpacing: 16
            
            KPICard {
                title: qsTr("Общий баланс")
                value: FinanceManager.totalBalance
                change: "+12%"
                changePositive: true
                icon: "\uf8fe"
                cardColor: "#4CAF50"
            }
            
            KPICard {
                title: qsTr("Доходы (мес)")
                value: FinanceManager.monthlyIncome
                change: "+8%"
                changePositive: true
                icon: "\ue5d8"
                cardColor: "#2196F3"
            }
            
            KPICard {
                title: qsTr("Расходы (мес)")
                value: FinanceManager.monthlyExpense
                change: "-5%"
                changePositive: true
                icon: "\ue5db"
                cardColor: "#FF9800"
            }
            
            KPICard {
                title: qsTr("Активные проекты")
                value: ProjectManager.activeCount
                subtitle: qsTr("из %1 всего").arg(ProjectManager.totalCount)
                icon: "\ue2c7"
                cardColor: "#9C27B0"
            }
        }
        
        // Charts row
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 24
            
            // Cash Flow Chart
            CFCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.6
                
                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 16
                    
                    Label {
                        text: qsTr("Движение денежных средств")
                        font.pixelSize: 16
                        font.bold: true
                    }
                    
                    ChartView {
                        id: cashFlowChart
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        antialiasing: true
                        legend.visible: true
                        legend.alignment: Qt.AlignBottom
                        animationOptions: ChartView.SeriesAnimations
                        
                        DateTimeAxis {
                            id: axisX
                            format: "dd.MM"
                            titleText: qsTr("Дата")
                        }
                        
                        ValueAxis {
                            id: axisY
                            titleText: qsTr("Сумма (₽)")
                            labelFormat: "%.0f"
                        }
                        
                        LineSeries {
                            name: qsTr("Доходы")
                            color: "#4CAF50"
                            width: 3
                            axisX: axisX
                            axisY: axisY
                        }
                        
                        LineSeries {
                            name: qsTr("Расходы")
                            color: "#F44336"
                            width: 3
                            axisX: axisX
                            axisY: axisY
                        }
                        
                        LineSeries {
                            name: qsTr("Баланс")
                            color: "#2196F3"
                            width: 2
                            style: Qt.DashLine
                            axisX: axisX
                            axisY: axisY
                        }
                    }
                }
            }
            
            // Right column
            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.4
                spacing: 24
                
                // Expenses by category
                CFCard {
                    Layout.fillWidth: true
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
                            legend.visible: false
                            
                            PieSeries {
                                id: expensePie
                                size: 0.8
                                holeSize: 0.4
                                
                                PieSlice { label: qsTr("Материалы"); value: 45; color: "#FF6384" }
                                PieSlice { label: qsTr("Зарплата"); value: 30; color: "#36A2EB" }
                                PieSlice { label: qsTr("Аренда"); value: 15; color: "#FFCE56" }
                                PieSlice { label: qsTr("Прочее"); value: 10; color: "#4BC0C0" }
                            }
                        }
                    }
                }
                
                // Recent transactions
                CFCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 250
                    
                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 16
                        
                        RowLayout {
                            Label {
                                text: qsTr("Последние операции")
                                font.pixelSize: 16
                                font.bold: true
                            }
                            Item { Layout.fillWidth: true }
                            Button {
                                text: qsTr("Все")
                                flat: true
                                onClicked: stackView.push("qrc:/qml/views/TransactionsView.qml")
                            }
                        }
                        
                        ListView {
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            spacing: 8
                            model: FinanceManager.recentTransactions
                            
                            delegate: Item {
                                width: parent.width
                                height: 50
                                
                                RowLayout {
                                    anchors.fill: parent
                                    spacing: 12
                                    
                                    Rectangle {
                                        Layout.preferredWidth: 40
                                        Layout.preferredHeight: 40
                                        radius: 20
                                        color: modelData.isIncome ? "#E8F5E9" : "#FFEBEE"
                                        
                                        Label {
                                            anchors.centerIn: parent
                                            text: modelData.isIncome ? "\ue5d8" : "\ue5db"
                                            font.family: "Material Icons"
                                            font.pixelSize: 20
                                            color: modelData.isIncome ? "#4CAF50" : "#F44336"
                                        }
                                    }
                                    
                                    ColumnLayout {
                                        Layout.fillWidth: true
                                        spacing: 2
                                        
                                        Label {
                                            text: modelData.category || (modelData.isIncome ? qsTr("Доход") : qsTr("Расход"))
                                            font.pixelSize: 14
                                        }
                                        Label {
                                            text: modelData.formattedDate
                                            font.pixelSize: 12
                                            color: Material.color(Material.Grey)
                                        }
                                    }
                                    
                                    Label {
                                        text: modelData.formattedAmount
                                        font.pixelSize: 14
                                        font.bold: true
                                        color: modelData.isIncome ? "#4CAF50" : "#F44336"
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        
        // Projects table
        CFCard {
            Layout.fillWidth: true
            Layout.preferredHeight: 300
            
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 16
                
                RowLayout {
                    Label {
                        text: qsTr("Активные проекты")
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Item { Layout.fillWidth: true }
                    CFButton {
                        text: qsTr("Новый проект")
                        icon: "\ue145"
                        highlighted: true
                        onClicked: newProjectDialog.open()
                    }
                }
                
                // Projects list
                ListView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    clip: true
                    spacing: 8
                    model: ProjectManager.activeProjects
                    
                    delegate: Item {
                        width: parent.width
                        height: 60
                        
                        MouseArea {
                            anchors.fill: parent
                            onClicked: {
                                // Open project detail
                            }
                        }
                        
                        RowLayout {
                            anchors.fill: parent
                            spacing: 16
                            
                            ColumnLayout {
                                Layout.fillWidth: true
                                spacing: 4
                                
                                Label {
                                    text: modelData.name
                                    font.pixelSize: 14
                                    font.bold: true
                                }
                                Label {
                                    text: modelData.code + " | " + modelData.statusLabel
                                    font.pixelSize: 12
                                    color: Material.color(Material.Grey)
                                }
                            }
                            
                            ColumnLayout {
                                Layout.preferredWidth: 150
                                spacing: 4
                                
                                ProgressBar {
                                    Layout.fillWidth: true
                                    value: modelData.budget > 0 ? modelData.budgetSpent / modelData.budget : 0
                                    Material.accent: value > 1 ? Material.Red : (value > 0.8 ? Material.Orange : Material.Green)
                                }
                                Label {
                                    text: modelData.formattedSpent + " / " + modelData.formattedBudget
                                    font.pixelSize: 11
                                    color: Material.color(Material.Grey)
                                }
                            }
                            
                            Label {
                                text: modelData.progress.toFixed(0) + "%"
                                font.pixelSize: 14
                                font.bold: true
                                color: modelData.progress >= 100 ? "#4CAF50" : Material.foreground
                            }
                        }
                    }
                }
            }
        }
    }
    
    Component.onCompleted: {
        loadChartData()
    }
    
    function loadChartData() {
        var cashFlow = FinanceManager.getCashFlowData(30)
        
        var incomeSeries = cashFlowChart.series(0)
        var expenseSeries = cashFlowChart.series(1)
        var balanceSeries = cashFlowChart.series(2)
        
        incomeSeries.clear()
        expenseSeries.clear()
        balanceSeries.clear()
        
        for (var i = 0; i < cashFlow.length; i++) {
            var point = cashFlow[i]
            var timestamp = new Date(point.date).getTime()
            incomeSeries.append(timestamp, point.income)
            expenseSeries.append(timestamp, point.expense)
            balanceSeries.append(timestamp, point.balance)
        }
        
        if (cashFlow.length > 0) {
            axisX.min = new Date(cashFlow[0].date)
            axisX.max = new Date(cashFlow[cashFlow.length - 1].date)
        }
    }
}
