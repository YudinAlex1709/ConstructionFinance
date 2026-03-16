#include "application.h"
#include "constants.h"
#include <QStandardPaths>
#include <QDir>
#include <QLocale>

Application* Application::s_instance = nullptr;

Application* Application::instance()
{
    if (!s_instance) {
        s_instance = new Application();
    }
    return s_instance;
}

Application::Application(QObject *parent)
    : QObject(parent)
{
}

QString Application::version() const
{
    return Constants::APP_VERSION;
}

bool Application::isDebugBuild() const
{
#ifdef DEBUG_BUILD
    return true;
#else
    return false;
#endif
}

QString Application::formatFileSize(qint64 bytes)
{
    const QStringList units = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    double size = bytes;
    
    while (size >= 1024.0 && unitIndex < units.size() - 1) {
        size /= 1024.0;
        unitIndex++;
    }
    
    return QString("%1 %2").arg(size, 0, 'f', 2).arg(units[unitIndex]);
}

QString Application::getDatabasePath()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return dataDir + "/" + Constants::DB_NAME;
}

QString Application::getBackupPath()
{
    QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QString backupDir = dataDir + "/backups";
    QDir().mkpath(backupDir);
    return backupDir;
}
