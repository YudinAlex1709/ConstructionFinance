import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import ConstructionFinance 1.0

Dialog {
    id: root
    title: qsTr("Новый проект")
    modal: true
    anchors.centerIn: parent
    width: 500
    standardButtons: Dialog.Save | Dialog.Cancel
    
    property var editProject: null
    
    onAccepted: {
        var data = {
            code: codeField.text,
            name: nameField.text,
            description: descriptionField.text,
            address: addressField.text,
            city: cityField.text,
            budget: parseFloat(budgetField.text) || 0,
            startDate: startDateField.selectedDate,
            endDatePlanned: endDateField.selectedDate,
            status: statusCombo.currentIndex
        }
        
        if (editProject) {
            ProjectManager.updateProject(editProject.id, data)
        } else {
            ProjectManager.createProject(data)
        }
    }
    
    ColumnLayout {
        width: parent.width
        spacing: 16
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            TextField {
                id: codeField
                Layout.fillWidth: true
                placeholderText: qsTr("Код проекта")
            }
            
            ComboBox {
                id: statusCombo
                Layout.preferredWidth: 150
                model: [qsTr("Планирование"), qsTr("Активный"), qsTr("Приостановлен"), qsTr("Завершен"), qsTr("Отменен")]
            }
        }
        
        TextField {
            id: nameField
            Layout.fillWidth: true
            placeholderText: qsTr("Название проекта")
        }
        
        TextField {
            id: descriptionField
            Layout.fillWidth: true
            placeholderText: qsTr("Описание")
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            TextField {
                id: addressField
                Layout.fillWidth: true
                placeholderText: qsTr("Адрес")
            }
            
            TextField {
                id: cityField
                Layout.preferredWidth: 150
                placeholderText: qsTr("Город")
            }
        }
        
        TextField {
            id: budgetField
            Layout.fillWidth: true
            placeholderText: qsTr("Бюджет (₽)")
            validator: DoubleValidator { bottom: 0; decimals: 2 }
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            ColumnLayout {
                Layout.fillWidth: true
                
                Label {
                    text: qsTr("Дата начала")
                    font.pixelSize: 12
                    color: Material.color(Material.Grey)
                }
                
                DatePicker {
                    id: startDateField
                    Layout.fillWidth: true
                }
            }
            
            ColumnLayout {
                Layout.fillWidth: true
                
                Label {
                    text: qsTr("Плановая дата окончания")
                    font.pixelSize: 12
                    color: Material.color(Material.Grey)
                }
                
                DatePicker {
                    id: endDateField
                    Layout.fillWidth: true
                }
            }
        }
    }
    
    function setProject(project) {
        editProject = project
        codeField.text = project.code
        nameField.text = project.name
        descriptionField.text = project.description
        addressField.text = project.address
        cityField.text = project.city
        budgetField.text = project.budget.toString()
        statusCombo.currentIndex = project.status
        startDateField.selectedDate = project.startDate
        endDateField.selectedDate = project.endDatePlanned
    }
    
    function clear() {
        editProject = null
        codeField.text = ""
        nameField.text = ""
        descriptionField.text = ""
        addressField.text = ""
        cityField.text = ""
        budgetField.text = ""
        statusCombo.currentIndex = 0
        startDateField.selectedDate = new Date()
        endDateField.selectedDate = new Date()
    }
    
    onOpened: clear()
}
