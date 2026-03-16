#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariantMap>

class Database : public QObject
{
    Q_OBJECT

public:
    static Database* instance();
    
    bool initialize(const QString &dbPath = QString());
    bool isOpen() const;
    QSqlError lastError() const;
    
    QSqlQuery execute(const QString &sql, const QVariantMap &params = QVariantMap());
    bool executeBatch(const QString &sql, const QList<QVariantMap> &paramList);
    
    bool transaction();
    bool commit();
    bool rollback();
    
    // Utility methods
    Q_INVOKABLE QVariant queryValue(const QString &sql, const QVariantMap &params = QVariantMap());
    Q_INVOKABLE int queryInt(const QString &sql, const QVariantMap &params = QVariantMap());
    Q_INVOKABLE double queryDouble(const QString &sql, const QVariantMap &params = QVariantMap());
    Q_INVOKABLE QString queryString(const QString &sql, const QVariantMap &params = QVariantMap());
    
    // Backup and restore
    bool backup(const QString &backupPath);
    bool restore(const QString &backupPath);
    
    // Maintenance
    bool vacuum();
    bool analyze();
    qint64 databaseSize() const;

signals:
    void initialized();
    void error(const QString &message);
    void backupCompleted(const QString &path);
    void restoreCompleted(const QString &path);

private:
    explicit Database(QObject *parent = nullptr);
    ~Database();
    
    bool open(const QString &dbPath);
    void close();
    bool setupPragmas();
    
    QSqlDatabase m_db;
    QString m_dbPath;
    static Database *s_instance;
};

#endif // DATABASE_H
