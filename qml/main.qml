import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import QtQuick.Window
import ConstructionFinance 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1400
    height: 900
    minimumWidth: 1200
    minimumHeight: 700
    title: "Construction Finance"
    
    Material.theme: Settings.theme === "dark" ? Material.Dark : Material.Light
    Material.primary: Material.Indigo
    Material.accent: Material.Blue
    
    // Login check
    Component.onCompleted: {
        if (!UserManager.isLoggedIn) {
            loginDialog.open()
        }
    }
    
    // Main layout
    RowLayout {
        anchors.fill: parent
        spacing: 0
        
        // Sidebar
        Sidebar {
            id: sidebar
            Layout.preferredWidth: 260
            Layout.fillHeight: true
            onMenuItemClicked: function(view) {
                stackView.replace("qrc:/qml/views/" + view + ".qml")
            }
        }
        
        // Content area
        StackView {
            id: stackView
            Layout.fillWidth: true
            Layout.fillHeight: true
            initialItem: "qrc:/qml/views/DashboardView.qml"
        }
    }
    
    // Login Dialog
    Dialog {
        id: loginDialog
        title: qsTr("Вход в систему")
        modal: true
        closePolicy: Popup.NoAutoClose
        anchors.centerIn: parent
        width: 400
        
        ColumnLayout {
            width: parent.width
            spacing: 16
            
            TextField {
                id: emailField
                Layout.fillWidth: true
                placeholderText: qsTr("Email")
                text: "admin@construction.finance"
            }
            
            TextField {
                id: passwordField
                Layout.fillWidth: true
                placeholderText: qsTr("Пароль")
                echoMode: TextInput.Password
                text: "admin"
            }
            
            Label {
                id: errorLabel
                color: Material.color(Material.Red)
                visible: false
            }
            
            Button {
                text: qsTr("Войти")
                highlighted: true
                Layout.fillWidth: true
                onClicked: {
                    if (UserManager.login(emailField.text, passwordField.text)) {
                        loginDialog.close()
                        errorLabel.visible = false
                    } else {
                        errorLabel.text = qsTr("Неверный email или пароль")
                        errorLabel.visible = true
                    }
                }
            }
        }
    }
    
    // New Project Dialog
    NewProjectDialog {
        id: newProjectDialog
    }
    
    // New Transaction Dialog
    NewTransactionDialog {
        id: newTransactionDialog
    }
}
