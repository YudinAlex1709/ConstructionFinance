#ifndef MIGRATION_H
#define MIGRATION_H

#include <QString>
#include <QList>

class Migration
{
public:
    Migration();
    
    bool run();
    QString lastError() const { return m_lastError; }
    int currentVersion() const;
    
private:
    bool createVersionTable();
    bool applyMigration(int version, const QString &sql);
    bool setVersion(int version);
    
    QString m_lastError;
    
    static const QList<QPair<int, QString>> MIGRATIONS;
};

#endif // MIGRATION_H
