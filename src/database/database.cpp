#include "database.h"
#include "migration.h"
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QDateTime>

Database* Database::s_instance = nullptr;

Database* Database::instance()
{
    if (!s_instance) {
        s_instance = new Database();
    }
    return s_instance;
}

Database::Database(QObject *parent)
    : QObject(parent)
{
}

Database::~Database()
{
    close();
}

bool Database::initialize(const QString &dbPath)
{
    QString path = dbPath;
    if (path.isEmpty()) {
        // Use default location
        QString dataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
        QDir().mkpath(dataDir);
        path = dataDir + "/construction_finance.db";
    }
    
    m_dbPath = path;
    
    if (!open(path)) {
        emit error(tr("Не удалось открыть базу данных: %1").arg(lastError().text()));
        return false;
    }
    
    if (!setupPragmas()) {
        qWarning() << "Failed to setup pragmas";
    }
    
    // Run migrations
    Migration migration;
    if (!migration.run()) {
        emit error(tr("Ошибка миграции базы данных: %1").arg(migration.lastError()));
        return false;
    }
    
    emit initialized();
    return true;
}

bool Database::open(const QString &dbPath)
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(dbPath);
    
    if (!m_db.open()) {
        qCritical() << "Failed to open database:" << m_db.lastError().text();
        return false;
    }
    
    qDebug() << "Database opened:" << dbPath;
    return true;
}

void Database::close()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
}

bool Database::setupPragmas()
{
    QSqlQuery query;
    
    // Enable foreign keys
    if (!query.exec("PRAGMA foreign_keys = ON")) {
        qWarning() << "Failed to enable foreign keys";
    }
    
    // Set journal mode to WAL for better concurrency
    if (!query.exec("PRAGMA journal_mode = WAL")) {
        qWarning() << "Failed to set WAL mode";
    }
    
    // Set synchronous mode to NORMAL for better performance
    if (!query.exec("PRAGMA synchronous = NORMAL")) {
        qWarning() << "Failed to set synchronous mode";
    }
    
    // Set cache size
    if (!query.exec("PRAGMA cache_size = 10000")) {
        qWarning() << "Failed to set cache size";
    }
    
    // Set temp store to memory
    if (!query.exec("PRAGMA temp_store = MEMORY")) {
        qWarning() << "Failed to set temp store";
    }
    
    return true;
}

bool Database::isOpen() const
{
    return m_db.isOpen();
}

QSqlError Database::lastError() const
{
    return m_db.lastError();
}

QSqlQuery Database::execute(const QString &sql, const QVariantMap &params)
{
    QSqlQuery query(m_db);
    query.prepare(sql);
    
    for (auto it = params.begin(); it != params.end(); ++it) {
        query.bindValue(it.key(), it.value());
    }
    
    if (!query.exec()) {
        qWarning() << "Query failed:" << query.lastError().text();
        qWarning() << "SQL:" << sql;
    }
    
    return query;
}

bool Database::executeBatch(const QString &sql, const QList<QVariantMap> &paramList)
{
    if (!transaction()) {
        return false;
    }
    
    bool success = true;
    for (const auto &params : paramList) {
        QSqlQuery query = execute(sql, params);
        if (query.lastError().isValid()) {
            success = false;
            break;
        }
    }
    
    if (success) {
        return commit();
    } else {
        rollback();
        return false;
    }
}

bool Database::transaction()
{
    return m_db.transaction();
}

bool Database::commit()
{
    return m_db.commit();
}

bool Database::rollback()
{
    return m_db.rollback();
}

QVariant Database::queryValue(const QString &sql, const QVariantMap &params)
{
    QSqlQuery query = execute(sql, params);
    if (query.next()) {
        return query.value(0);
    }
    return QVariant();
}

int Database::queryInt(const QString &sql, const QVariantMap &params)
{
    return queryValue(sql, params).toInt();
}

double Database::queryDouble(const QString &sql, const QVariantMap &params)
{
    return queryValue(sql, params).toDouble();
}

QString Database::queryString(const QString &sql, const QVariantMap &params)
{
    return queryValue(sql, params).toString();
}

bool Database::backup(const QString &backupPath)
{
    QString path = backupPath;
    if (path.isEmpty()) {
        QString backupDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/backups";
        QDir().mkpath(backupDir);
        path = backupDir + "/backup_" + QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss") + ".db";
    }
    
    // Use SQLite backup API
    QSqlQuery query;
    query.prepare("VACUUM INTO :path");
    query.bindValue(":path", path);
    
    if (!query.exec()) {
        emit error(tr("Ошибка создания резервной копии: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit backupCompleted(path);
    return true;
}

bool Database::restore(const QString &backupPath)
{
    if (!QFile::exists(backupPath)) {
        emit error(tr("Файл резервной копии не найден"));
        return false;
    }
    
    // Close current database
    close();
    
    // Copy backup to current location
    if (!QFile::remove(m_dbPath)) {
        qWarning() << "Failed to remove current database";
    }
    
    if (!QFile::copy(backupPath, m_dbPath)) {
        emit error(tr("Ошибка восстановления резервной копии"));
        return false;
    }
    
    // Reopen database
    if (!open(m_dbPath)) {
        emit error(tr("Ошибка открытия восстановленной базы данных"));
        return false;
    }
    
    emit restoreCompleted(m_dbPath);
    return true;
}

bool Database::vacuum()
{
    QSqlQuery query;
    if (!query.exec("VACUUM")) {
        emit error(tr("Ошибка оптимизации базы данных: %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

bool Database::analyze()
{
    QSqlQuery query;
    if (!query.exec("ANALYZE")) {
        emit error(tr("Ошибка анализа базы данных: %1").arg(query.lastError().text()));
        return false;
    }
    return true;
}

qint64 Database::databaseSize() const
{
    QFile file(m_dbPath);
    return file.size();
}
