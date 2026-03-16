import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import ConstructionFinance 1.0

Rectangle {
    id: root
    color: Material.primary
    
    signal menuItemClicked(string view)
    
    ColumnLayout {
        anchors.fill: parent
        spacing: 0
        
        // Logo header
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: Qt.darker(Material.primary, 1.2)
            
            Row {
                anchors.centerIn: parent
                spacing: 12
                
                Rectangle {
                    width: 40
                    height: 40
                    radius: 8
                    color: Material.accent
                    
                    Label {
                        anchors.centerIn: parent
                        text: "CF"
                        color: "white"
                        font.bold: true
                        font.pixelSize: 18
                    }
                }
                
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    
                    Label {
                        text: "Construction"
                        color: "white"
                        font.pixelSize: 16
                        font.bold: true
                    }
                    Label {
                        text: "Finance"
                        color: "white"
                        font.pixelSize: 12
                        opacity: 0.8
                    }
                }
            }
        }
        
        // Menu items
        ListView {
            id: menuList
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: [
                { icon: "\ue871", text: qsTr("Дашборд"), view: "DashboardView" },
                { icon: "\ue2c7", text: qsTr("Проекты"), view: "ProjectsView" },
                { icon: "\ue84f", text: qsTr("Финансы"), view: "FinanceView" },
                { icon: "\ue8b0", text: qsTr("Операции"), view: "TransactionsView" },
                { icon: "\ue6e1", text: qsTr("Отчеты"), view: "ReportsView" },
                { icon: "\ue0af", text: qsTr("Контрагенты"), view: "ContractorsView" },
                { icon: "\ue8b8", text: qsTr("Настройки"), view: "SettingsView" }
            ]
            spacing: 4
            currentIndex: 0
            clip: true
            
            delegate: ItemDelegate {
                width: menuList.width
                height: 50
                highlighted: menuList.currentIndex === index
                
                contentItem: Row {
                    spacing: 16
                    leftPadding: 20
                    
                    Label {
                        text: modelData.icon
                        font.family: "Material Icons"
                        font.pixelSize: 24
                        color: highlighted ? Material.accent : "white"
                        anchors.verticalCenter: parent.verticalCenter
                    }
                    
                    Label {
                        text: modelData.text
                        color: highlighted ? Material.accent : "white"
                        font.pixelSize: 14
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                
                background: Rectangle {
                    color: highlighted ? Qt.rgba(1, 1, 1, 0.1) : "transparent"
                }
                
                onClicked: {
                    menuList.currentIndex = index
                    root.menuItemClicked(modelData.view)
                }
            }
        }
        
        // User info footer
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 70
            color: Qt.darker(Material.primary, 1.1)
            
            RowLayout {
                anchors.fill: parent
                anchors.margins: 12
                spacing: 12
                
                Rectangle {
                    Layout.preferredWidth: 40
                    Layout.preferredHeight: 40
                    radius: 20
                    color: Material.accent
                    
                    Label {
                        anchors.centerIn: parent
                        text: UserManager.currentUser ? UserManager.currentUser.initials : "?"
                        color: "white"
                        font.bold: true
                    }
                }
                
                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 2
                    
                    Label {
                        text: UserManager.currentUser ? UserManager.currentUser.fullName : qsTr("Гость")
                        color: "white"
                        font.pixelSize: 13
                        elide: Text.ElideRight
                    }
                    Label {
                        text: UserManager.currentUser ? UserManager.currentUser.roleLabel : ""
                        color: Qt.rgba(1, 1, 1, 0.7)
                        font.pixelSize: 11
                    }
                }
                
                ToolButton {
                    text: "\ue879"
                    font.family: "Material Icons"
                    font.pixelSize: 20
                    contentItem: Label {
                        text: parent.text
                        font: parent.font
                        color: "white"
                        anchors.centerIn: parent
                    }
                    onClicked: UserManager.logout()
                }
            }
        }
    }
}
