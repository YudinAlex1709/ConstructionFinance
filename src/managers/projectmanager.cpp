#include "projectmanager.h"
#include "../models/project.h"
#include "../database/repositories/projectrepository.h"
#include <QDebug>

ProjectManager* ProjectManager::s_instance = nullptr;

ProjectManager* ProjectManager::instance()
{
    if (!s_instance) {
        s_instance = new ProjectManager();
    }
    return s_instance;
}

ProjectManager::ProjectManager(QObject *parent)
    : QObject(parent)
{
    loadProjects();
}

QVariantList ProjectManager::projects() const
{
    QVariantList list;
    for (Project *project : m_projects) {
        list.append(project->toMap());
    }
    return list;
}

QVariantList ProjectManager::activeProjects() const
{
    QVariantList list;
    for (Project *project : m_projects) {
        if (project->status() == Project::Active || 
            project->status() == Project::Planning) {
            list.append(project->toMap());
        }
    }
    return list;
}

void ProjectManager::loadProjects()
{
    // Clear existing projects
    qDeleteAll(m_projects);
    m_projects.clear();
    
    // Load from repository
    m_projects = ProjectRepository::instance()->getAll("default");
    
    updateStatistics();
    emit projectsChanged();
}

void ProjectManager::loadActiveProjects()
{
    qDeleteAll(m_projects);
    m_projects.clear();
    
    m_projects = ProjectRepository::instance()->getActive("default");
    
    emit projectsChanged();
}

Project* ProjectManager::createProject(const QVariantMap &data)
{
    Project *project = new Project();
    project->fromMap(data);
    project->setCompanyId("default");
    
    if (!project->save()) {
        delete project;
        emit error(tr("Не удалось создать проект"));
        return nullptr;
    }
    
    m_projects.append(project);
    updateStatistics();
    
    emit projectsChanged();
    emit projectCreated(project->id());
    
    return project;
}

bool ProjectManager::updateProject(const QString &id, const QVariantMap &data)
{
    Project *project = getProject(id);
    if (!project) {
        emit error(tr("Проект не найден"));
        return false;
    }
    
    project->fromMap(data);
    
    if (!project->save()) {
        emit error(tr("Не удалось обновить проект"));
        return false;
    }
    
    updateStatistics();
    emit projectsChanged();
    emit projectUpdated(id);
    
    return true;
}

bool ProjectManager::deleteProject(const QString &id)
{
    Project *project = getProject(id);
    if (!project) {
        emit error(tr("Проект не найден"));
        return false;
    }
    
    if (!project->remove()) {
        emit error(tr("Не удалось удалить проект"));
        return false;
    }
    
    m_projects.removeOne(project);
    delete project;
    
    updateStatistics();
    emit projectsChanged();
    emit projectDeleted(id);
    
    return true;
}

Project* ProjectManager::getProject(const QString &id)
{
    for (Project *project : m_projects) {
        if (project->id() == id) {
            return project;
        }
    }
    return nullptr;
}

void ProjectManager::searchProjects(const QString &query)
{
    qDeleteAll(m_projects);
    m_projects.clear();
    
    if (query.isEmpty()) {
        loadProjects();
        return;
    }
    
    m_projects = ProjectRepository::instance()->search(query, "default");
    emit projectsChanged();
}

QVariantMap ProjectManager::getProjectStatistics(const QString &projectId)
{
    QVariantMap stats;
    
    Project *project = getProject(projectId);
    if (!project) {
        return stats;
    }
    
    stats["budget"] = project->budget();
    stats["spent"] = project->budgetSpent();
    stats["remaining"] = project->budgetRemaining();
    stats["utilization"] = project->budgetUtilization();
    stats["progress"] = project->progress();
    stats["isOverBudget"] = project->isOverBudget();
    stats["daysRemaining"] = project->daysRemaining();
    
    return stats;
}

QVariantList ProjectManager::getProjectTransactions(const QString &projectId)
{
    // This would be implemented with TransactionRepository
    return QVariantList();
}

QVariantList ProjectManager::getProjectMaterials(const QString &projectId)
{
    // This would be implemented with MaterialRepository
    return QVariantList();
}

void ProjectManager::updateStatistics()
{
    QVariantMap stats = ProjectRepository::instance()->getStatistics("default");
    
    m_totalCount = stats["total"].toInt();
    m_activeCount = stats["active"].toInt();
    m_planningCount = stats["planning"].toInt();
    m_completedCount = stats["completed"].toInt();
    m_totalBudget = stats["totalBudget"].toDouble();
    m_totalSpent = stats["totalSpent"].toDouble();
    
    emit statisticsChanged();
}
