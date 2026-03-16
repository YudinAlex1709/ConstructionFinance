#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QIcon>
#include <QFontDatabase>
#include <QTranslator>
#include <QLocale>
#include <QDebug>

#include "app/application.h"
#include "app/settings.h"

#include "managers/projectmanager.h"
#include "managers/financemanager.h"
#include "managers/reportmanager.h"
#include "managers/usermanager.h"

#include "models/project.h"
#include "models/transaction.h"
#include "models/contractor.h"
#include "models/user.h"
#include "models/bankaccount.h"
#include "models/category.h"
#include "models/material.h"

#include "threed/scene3d.h"
#include "threed/buildingmodel.h"

#include "database/database.h"
#include "utils/currencyutils.h"
#include "utils/dateutils.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    
    // Application settings
    app.setOrganizationName("ConstructionFinance");
    app.setApplicationName("Construction Finance");
    app.setApplicationVersion("1.0.0");
    
    // Load translations
    QTranslator translator;
    QString locale = Settings::instance()->language();
    if (translator.load(":/i18n/" + locale + ".qm")) {
        app.installTranslator(&translator);
    }
    
    // Initialize database
    if (!Database::instance()->initialize()) {
        qCritical() << "Failed to initialize database:" << Database::instance()->lastError().text();
        return -1;
    }
    
    qDebug() << "Database initialized at:" << Application::instance()->getDatabasePath();
    
    // Register types for QML
    qmlRegisterType<Project>("ConstructionFinance", 1, 0, "Project");
    qmlRegisterType<Transaction>("ConstructionFinance", 1, 0, "Transaction");
    qmlRegisterType<Contractor>("ConstructionFinance", 1, 0, "Contractor");
    qmlRegisterType<User>("ConstructionFinance", 1, 0, "User");
    qmlRegisterType<BankAccount>("ConstructionFinance", 1, 0, "BankAccount");
    qmlRegisterType<Category>("ConstructionFinance", 1, 0, "Category");
    qmlRegisterType<Material>("ConstructionFinance", 1, 0, "Material");
    qmlRegisterType<Scene3D>("ConstructionFinance", 1, 0, "Scene3D");
    qmlRegisterType<BuildingModel>("ConstructionFinance", 1, 0, "BuildingModel");
    
    // Register enums
    qmlRegisterUncreatableType<Project>("ConstructionFinance", 1, 0, "ProjectStatus", "Enum only");
    qmlRegisterUncreatableType<Transaction>("ConstructionFinance", 1, 0, "TransactionType", "Enum only");
    qmlRegisterUncreatableType<Transaction>("ConstructionFinance", 1, 0, "TransactionStatus", "Enum only");
    qmlRegisterUncreatableType<Contractor>("ConstructionFinance", 1, 0, "ContractorType", "Enum only");
    qmlRegisterUncreatableType<User>("ConstructionFinance", 1, 0, "UserRole", "Enum only");
    qmlRegisterUncreatableType<Category>("ConstructionFinance", 1, 0, "CategoryType", "Enum only");
    qmlRegisterUncreatableType<Scene3D>("ConstructionFinance", 1, 0, "ViewMode", "Enum only");
    qmlRegisterUncreatableType<ReportManager>("ConstructionFinance", 1, 0, "ReportType", "Enum only");
    
    // Register singletons
    qmlRegisterSingletonType<ProjectManager>(
        "ConstructionFinance", 1, 0, "ProjectManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return ProjectManager::instance();
        });
    
    qmlRegisterSingletonType<FinanceManager>(
        "ConstructionFinance", 1, 0, "FinanceManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return FinanceManager::instance();
        });
    
    qmlRegisterSingletonType<ReportManager>(
        "ConstructionFinance", 1, 0, "ReportManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return ReportManager::instance();
        });
    
    qmlRegisterSingletonType<UserManager>(
        "ConstructionFinance", 1, 0, "UserManager",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return UserManager::instance();
        });
    
    qmlRegisterSingletonType<Settings>(
        "ConstructionFinance", 1, 0, "Settings",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return Settings::instance();
        });
    
    qmlRegisterSingletonType<Application>(
        "ConstructionFinance", 1, 0, "App",
        [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject* {
            Q_UNUSED(engine)
            Q_UNUSED(scriptEngine)
            return Application::instance();
        });
    
    // Load QML
    QQmlApplicationEngine engine;
    
    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    
    engine.load(url);
    
    return app.exec();
}
