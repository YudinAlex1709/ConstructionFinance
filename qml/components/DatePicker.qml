import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts

TextField {
    id: root
    
    property date selectedDate: new Date()
    
    text: selectedDate.toLocaleDateString(Qt.locale("ru_RU"), "dd.MM.yyyy")
    readOnly: true
    
    MouseArea {
        anchors.fill: parent
        onClicked: calendarPopup.open()
    }
    
    Popup {
        id: calendarPopup
        width: 300
        height: 320
        modal: true
        anchors.centerIn: parent
        
        ColumnLayout {
            anchors.fill: parent
            spacing: 8
            
            // Month/Year header
            RowLayout {
                Layout.fillWidth: true
                
                ToolButton {
                    text: "\ue5cb"
                    font.family: "Material Icons"
                    onClicked: calendar.showPreviousMonth()
                }
                
                Label {
                    Layout.fillWidth: true
                    text: calendar.title
                    horizontalAlignment: Text.AlignHCenter
                    font.bold: true
                }
                
                ToolButton {
                    text: "\ue5cc"
                    font.family: "Material Icons"
                    onClicked: calendar.showNextMonth()
                }
            }
            
            // Weekday headers
            RowLayout {
                Layout.fillWidth: true
                
                Repeater {
                    model: [qsTr("Пн"), qsTr("Вт"), qsTr("Ср"), qsTr("Чт"), qsTr("Пт"), qsTr("Сб"), qsTr("Вс")]
                    
                    Label {
                        Layout.fillWidth: true
                        text: modelData
                        horizontalAlignment: Text.AlignHCenter
                        font.pixelSize: 11
                        color: Material.color(Material.Grey)
                    }
                }
            }
            
            // Calendar grid
            GridLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                columns: 7
                rows: 6
                
                Repeater {
                    id: dayRepeater
                    model: 42
                    
                    delegate: ItemDelegate {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        
                        property date dayDate: getDateForIndex(index)
                        property bool isCurrentMonth: dayDate.getMonth() === calendar.visibleMonth
                        property bool isSelected: dayDate.getTime() === root.selectedDate.getTime()
                        
                        text: dayDate.getDate()
                        enabled: isCurrentMonth
                        highlighted: isSelected
                        
                        contentItem: Label {
                            text: parent.text
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            color: {
                                if (!parent.enabled) return Material.color(Material.Grey, Material.Shade400)
                                if (parent.isSelected) return "white"
                                return Material.foreground
                            }
                        }
                        
                        background: Rectangle {
                            color: parent.isSelected ? Material.accent : "transparent"
                            radius: width / 2
                        }
                        
                        onClicked: {
                            root.selectedDate = dayDate
                            calendarPopup.close()
                        }
                    }
                }
            }
        }
    }
    
    QtObject {
        id: calendar
        property int visibleYear: new Date().getFullYear()
        property int visibleMonth: new Date().getMonth()
        property string title: Qt.locale("ru_RU").monthName(visibleMonth) + " " + visibleYear
        
        function showPreviousMonth() {
            visibleMonth--
            if (visibleMonth < 0) {
                visibleMonth = 11
                visibleYear--
            }
            updateDayRepeater()
        }
        
        function showNextMonth() {
            visibleMonth++
            if (visibleMonth > 11) {
                visibleMonth = 0
                visibleYear++
            }
            updateDayRepeater()
        }
        
        function updateDayRepeater() {
            dayRepeater.model = 0
            dayRepeater.model = 42
        }
    }
    
    function getDateForIndex(index) {
        var firstDayOfMonth = new Date(calendar.visibleYear, calendar.visibleMonth, 1)
        var startOfCalendar = new Date(firstDayOfMonth)
        startOfCalendar.setDate(startOfCalendar.getDate() - (firstDayOfMonth.getDay() + 6) % 7)
        
        var result = new Date(startOfCalendar)
        result.setDate(result.getDate() + index)
        return result
    }
}
