#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <QObject>
#include <QQmlListProperty>
#include <QVariantList>

class Project;

class ProjectManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList projects READ projects NOTIFY projectsChanged)
    Q_PROPERTY(QVariantList activeProjects READ activeProjects NOTIFY projectsChanged)
    Q_PROPERTY(int totalCount READ totalCount NOTIFY statisticsChanged)
    Q_PROPERTY(int activeCount READ activeCount NOTIFY statisticsChanged)
    Q_PROPERTY(int planningCount READ planningCount NOTIFY statisticsChanged)
    Q_PROPERTY(int completedCount READ completedCount NOTIFY statisticsChanged)
    Q_PROPERTY(double totalBudget READ totalBudget NOTIFY statisticsChanged)
    Q_PROPERTY(double totalSpent READ totalSpent NOTIFY statisticsChanged)
    Q_PROPERTY(double totalRemaining READ totalRemaining NOTIFY statisticsChanged)

public:
    static ProjectManager* instance();
    
    QVariantList projects() const;
    QVariantList activeProjects() const;
    int totalCount() const { return m_totalCount; }
    int activeCount() const { return m_activeCount; }
    int planningCount() const { return m_planningCount; }
    int completedCount() const { return m_completedCount; }
    double totalBudget() const { return m_totalBudget; }
    double totalSpent() const { return m_totalSpent; }
    double totalRemaining() const { return m_totalBudget - m_totalSpent; }
    
    Q_INVOKABLE void loadProjects();
    Q_INVOKABLE void loadActiveProjects();
    Q_INVOKABLE Project* createProject(const QVariantMap &data);
    Q_INVOKABLE bool updateProject(const QString &id, const QVariantMap &data);
    Q_INVOKABLE bool deleteProject(const QString &id);
    Q_INVOKABLE Project* getProject(const QString &id);
    Q_INVOKABLE void searchProjects(const QString &query);
    
    Q_INVOKABLE QVariantMap getProjectStatistics(const QString &projectId);
    Q_INVOKABLE QVariantList getProjectTransactions(const QString &projectId);
    Q_INVOKABLE QVariantList getProjectMaterials(const QString &projectId);

signals:
    void projectsChanged();
    void statisticsChanged();
    void projectCreated(const QString &id);
    void projectUpdated(const QString &id);
    void projectDeleted(const QString &id);
    void error(const QString &message);

private:
    explicit ProjectManager(QObject *parent = nullptr);
    void updateStatistics();
    
    QList<Project*> m_projects;
    int m_totalCount = 0;
    int m_activeCount = 0;
    int m_planningCount = 0;
    int m_completedCount = 0;
    double m_totalBudget = 0.0;
    double m_totalSpent = 0.0;
    
    static ProjectManager *s_instance;
};

#endif // PROJECTMANAGER_H
