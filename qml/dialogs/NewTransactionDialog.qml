import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import ConstructionFinance 1.0

Dialog {
    id: root
    title: qsTr("Новая операция")
    modal: true
    anchors.centerIn: parent
    width: 450
    standardButtons: Dialog.Save | Dialog.Cancel
    
    onAccepted: {
        var data = {
            type: typeCombo.currentIndex,
            category: categoryField.text,
            amount: parseFloat(amountField.text) || 0,
            date: dateField.selectedDate,
            projectId: projectCombo.currentIndex >= 0 ? projectCombo.model[projectCombo.currentIndex].id : "",
            contractorId: contractorField.text,
            description: descriptionField.text,
            documentNumber: documentNumberField.text,
            status: 1
        }
        
        FinanceManager.createTransaction(data)
    }
    
    ColumnLayout {
        width: parent.width
        spacing: 16
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            ComboBox {
                id: typeCombo
                Layout.fillWidth: true
                model: [qsTr("Доход"), qsTr("Расход"), qsTr("Перевод"), qsTr("Займ выдан"), qsTr("Займ получен")]
            }
            
            TextField {
                id: amountField
                Layout.preferredWidth: 150
                placeholderText: qsTr("Сумма")
                validator: DoubleValidator { bottom: 0; decimals: 2 }
            }
        }
        
        TextField {
            id: categoryField
            Layout.fillWidth: true
            placeholderText: qsTr("Категория")
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            ColumnLayout {
                Layout.fillWidth: true
                
                Label {
                    text: qsTr("Дата")
                    font.pixelSize: 12
                    color: Material.color(Material.Grey)
                }
                
                DatePicker {
                    id: dateField
                    Layout.fillWidth: true
                }
            }
            
            TextField {
                id: documentNumberField
                Layout.preferredWidth: 150
                placeholderText: qsTr("№ документа")
            }
        }
        
        ComboBox {
            id: projectCombo
            Layout.fillWidth: true
            textRole: "name"
            model: ProjectManager.projects
            placeholderText: qsTr("Выберите проект")
        }
        
        TextField {
            id: contractorField
            Layout.fillWidth: true
            placeholderText: qsTr("Контрагент")
        }
        
        TextField {
            id: descriptionField
            Layout.fillWidth: true
            placeholderText: qsTr("Описание")
        }
    }
    
    function clear() {
        typeCombo.currentIndex = 1 // Expense by default
        categoryField.text = ""
        amountField.text = ""
        dateField.selectedDate = new Date()
        documentNumberField.text = ""
        projectCombo.currentIndex = -1
        contractorField.text = ""
        descriptionField.text = ""
    }
    
    onOpened: clear()
}
