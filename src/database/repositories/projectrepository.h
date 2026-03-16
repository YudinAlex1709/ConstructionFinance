#ifndef PROJECTREPOSITORY_H
#define PROJECTREPOSITORY_H

#include <QObject>
#include <QList>
#include <QVariantMap>

class Project;

class ProjectRepository : public QObject
{
    Q_OBJECT

public:
    static ProjectRepository* instance();
    
    // CRUD operations
    Q_INVOKABLE Project* getById(const QString &id);
    Q_INVOKABLE QList<Project*> getAll(const QString &companyId = QString());
    Q_INVOKABLE QList<Project*> getByStatus(int status, const QString &companyId = QString());
    Q_INVOKABLE QList<Project*> getActive(const QString &companyId = QString());
    Q_INVOKABLE QList<Project*> search(const QString &query, const QString &companyId = QString());
    
    // Statistics
    Q_INVOKABLE int count(const QString &companyId = QString());
    Q_INVOKABLE int countByStatus(int status, const QString &companyId = QString());
    Q_INVOKABLE double totalBudget(const QString &companyId = QString());
    Q_INVOKABLE double totalSpent(const QString &companyId = QString());
    
    // Aggregations
    Q_INVOKABLE QVariantMap getStatistics(const QString &companyId = QString());
    Q_INVOKABLE QVariantList getBudgetByMonth(int months = 12, const QString &companyId = QString());
    
    // Financial operations
    Q_INVOKABLE bool updateBudgetSpent(const QString &projectId, double amount);
    Q_INVOKABLE bool updateProgress(const QString &projectId, double progress);

signals:
    void projectSaved(const QString &id);
    void projectRemoved(const QString &id);
    void error(const QString &message);

private:
    explicit ProjectRepository(QObject *parent = nullptr);
    static ProjectRepository *s_instance;
};

#endif // PROJECTREPOSITORY_H
