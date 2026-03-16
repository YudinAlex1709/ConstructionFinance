# Construction Finance - Управление финансами строительных компаний
# Qt6/QML/C++ приложение

QT += core gui qml quick sql charts 3dcore 3drender 3dinput 3dextras 3danimation network

CONFIG += c++17

# Для Windows/MinGW или Linux/GCC
QMAKE_CXXFLAGS += -std=c++17 -Wall -Wextra

TARGET = ConstructionFinance
TEMPLATE = app

# Исходники C++
SOURCES += \
    src/main.cpp \
    src/app/application.cpp \
    src/app/settings.cpp \
    src/models/basemodel.cpp \
    src/models/project.cpp \
    src/models/transaction.cpp \
    src/models/contractor.cpp \
    src/models/user.cpp \
    src/models/bankaccount.cpp \
    src/models/category.cpp \
    src/models/material.cpp \
    src/database/database.cpp \
    src/database/migration.cpp \
    src/database/repositories/projectrepository.cpp \
    src/database/repositories/transactionrepository.cpp \
    src/database/repositories/userrepository.cpp \
    src/managers/projectmanager.cpp \
    src/managers/financemanager.cpp \
    src/managers/reportmanager.cpp \
    src/managers/usermanager.cpp \
    src/utils/currencyutils.cpp \
    src/utils/dateutils.cpp \
    src/threed/scene3d.cpp \
    src/threed/buildingmodel.cpp \
    src/threed/cameracontroller.cpp

HEADERS += \
    src/app/application.h \
    src/app/constants.h \
    src/app/settings.h \
    src/models/basemodel.h \
    src/models/project.h \
    src/models/transaction.h \
    src/models/contractor.h \
    src/models/user.h \
    src/models/bankaccount.h \
    src/models/category.h \
    src/models/material.h \
    src/database/database.h \
    src/database/migration.h \
    src/database/repositories/projectrepository.h \
    src/database/repositories/transactionrepository.h \
    src/database/repositories/userrepository.h \
    src/managers/projectmanager.h \
    src/managers/financemanager.h \
    src/managers/reportmanager.h \
    src/managers/usermanager.h \
    src/utils/currencyutils.h \
    src/utils/dateutils.h \
    src/threed/scene3d.h \
    src/threed/buildingmodel.h \
    src/threed/cameracontroller.h

# QML файлы
QML_FILES += \
    qml/main.qml \
    qml/components/CFButton.qml \
    qml/components/CFCard.qml \
    qml/components/CFChart.qml \
    qml/components/CFTable.qml \
    qml/components/CFTextField.qml \
    qml/components/CFStatusBadge.qml \
    qml/components/CFMoneyDisplay.qml \
    qml/components/KPICard.qml \
    qml/components/TransactionListItem.qml \
    qml/components/ProjectsTable.qml \
    qml/views/DashboardView.qml \
    qml/views/ProjectsView.qml \
    qml/views/ProjectDetailView.qml \
    qml/views/FinanceView.qml \
    qml/views/TransactionsView.qml \
    qml/views/ReportsView.qml \
    qml/views/ContractorsView.qml \
    qml/views/SettingsView.qml \
    qml/views/threed/Building3DView.qml \
    qml/views/threed/ModelViewer.qml \
    qml/dialogs/NewProjectDialog.qml \
    qml/dialogs/NewTransactionDialog.qml \
    qml/dialogs/ContractorDialog.qml \
    qml/dialogs/ReportExportDialog.qml \
    qml/styles/CFStyle.qml

RESOURCES += resources.qrc

# Дополнительные библиотеки (если нужны)
# LIBS += -L/path/to/libs -llibrary

# Установка
unix {
    target.path = /usr/local/bin
    INSTALLS += target
}

# Для Windows
win32 {
    RC_FILE = resources/windows/app.rc
}

# Для macOS
macx {
    ICON = resources/macos/app.icns
    QMAKE_INFO_PLIST = resources/macos/Info.plist
}

# Дебаг/Релиз конфигурация
CONFIG(debug, debug|release) {
    DEFINES += DEBUG_BUILD
} else {
    DEFINES += RELEASE_BUILD
    # Оптимизации для релиза
    QMAKE_CXXFLAGS_RELEASE += -O3
}
