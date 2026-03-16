import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import ConstructionFinance 1.0

Dialog {
    id: root
    title: qsTr("Контрагент")
    modal: true
    anchors.centerIn: parent
    width: 500
    standardButtons: Dialog.Save | Dialog.Cancel
    
    ColumnLayout {
        width: parent.width
        spacing: 16
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            ComboBox {
                id: typeCombo
                Layout.fillWidth: true
                model: [qsTr("Заказчик"), qsTr("Поставщик"), qsTr("Субподрядчик"), qsTr("Сотрудник"), qsTr("Прочее")]
            }
        }
        
        TextField {
            id: nameField
            Layout.fillWidth: true
            placeholderText: qsTr("Название / ФИО")
        }
        
        TextField {
            id: fullNameField
            Layout.fillWidth: true
            placeholderText: qsTr("Полное наименование")
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            TextField {
                id: innField
                Layout.fillWidth: true
                placeholderText: qsTr("ИНН")
                validator: RegularExpressionValidator { regularExpression: /\d{10,12}/ }
            }
            
            TextField {
                id: kppField
                Layout.preferredWidth: 150
                placeholderText: qsTr("КПП")
                validator: RegularExpressionValidator { regularExpression: /\d{9}/ }
            }
        }
        
        RowLayout {
            Layout.fillWidth: true
            spacing: 16
            
            TextField {
                id: phoneField
                Layout.fillWidth: true
                placeholderText: qsTr("Телефон")
            }
            
            TextField {
                id: emailField
                Layout.fillWidth: true
                placeholderText: qsTr("Email")
            }
        }
        
        TextField {
            id: addressField
            Layout.fillWidth: true
            placeholderText: qsTr("Адрес")
        }
        
        GroupBox {
            Layout.fillWidth: true
            title: qsTr("Банковские реквизиты")
            
            ColumnLayout {
                anchors.fill: parent
                
                TextField {
                    id: bankNameField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Название банка")
                }
                
                TextField {
                    id: bankAccountField
                    Layout.fillWidth: true
                    placeholderText: qsTr("Расчетный счет")
                }
                
                TextField {
                    id: bikField
                    Layout.fillWidth: true
                    placeholderText: qsTr("БИК")
                }
            }
        }
        
        TextField {
            id: notesField
            Layout.fillWidth: true
            placeholderText: qsTr("Примечания")
        }
    }
}
