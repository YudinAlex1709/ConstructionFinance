#include "projectrepository.h"
#include "../../models/project.h"
#include "../database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

ProjectRepository* ProjectRepository::s_instance = nullptr;

ProjectRepository* ProjectRepository::instance()
{
    if (!s_instance) {
        s_instance = new ProjectRepository();
    }
    return s_instance;
}

ProjectRepository::ProjectRepository(QObject *parent)
    : QObject(parent)
{
}

Project* ProjectRepository::getById(const QString &id)
{
    QSqlQuery query = Database::instance()->execute(
        "SELECT p.*, c.name as client_name FROM projects p "
        "LEFT JOIN contractors c ON p.client_id = c.id "
        "WHERE p.id = :id",
        {{":id", id}}
    );
    
    if (query.next()) {
        Project *project = new Project();
        project->loadFromRecord(query.record());
        project->setClientName(query.value("client_name").toString());
        return project;
    }
    
    return nullptr;
}

QList<Project*> ProjectRepository::getAll(const QString &companyId)
{
    QList<Project*> projects;
    
    QString sql = 
        "SELECT p.*, c.name as client_name FROM projects p "
        "LEFT JOIN contractors c ON p.client_id = c.id ";
    
    QVariantMap params;
    if (!companyId.isEmpty()) {
        sql += "WHERE p.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY p.created_at DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Project *project = new Project();
        project->loadFromRecord(query.record());
        project->setClientName(query.value("client_name").toString());
        projects.append(project);
    }
    
    return projects;
}

QList<Project*> ProjectRepository::getByStatus(int status, const QString &companyId)
{
    QList<Project*> projects;
    
    QString sql = 
        "SELECT p.*, c.name as client_name FROM projects p "
        "LEFT JOIN contractors c ON p.client_id = c.id "
        "WHERE p.status = :status ";
    
    QVariantMap params;
    params[":status"] = status;
    
    if (!companyId.isEmpty()) {
        sql += "AND p.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY p.created_at DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Project *project = new Project();
        project->loadFromRecord(query.record());
        project->setClientName(query.value("client_name").toString());
        projects.append(project);
    }
    
    return projects;
}

QList<Project*> ProjectRepository::getActive(const QString &companyId)
{
    QList<Project*> projects;
    
    QString sql = 
        "SELECT p.*, c.name as client_name FROM projects p "
        "LEFT JOIN contractors c ON p.client_id = c.id "
        "WHERE p.status IN (0, 1, 2) ";  // Planning, Active, OnHold
    
    QVariantMap params;
    if (!companyId.isEmpty()) {
        sql += "AND p.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY p.created_at DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Project *project = new Project();
        project->loadFromRecord(query.record());
        project->setClientName(query.value("client_name").toString());
        projects.append(project);
    }
    
    return projects;
}

QList<Project*> ProjectRepository::search(const QString &query, const QString &companyId)
{
    QList<Project*> projects;
    
    QString sql = 
        "SELECT p.*, c.name as client_name FROM projects p "
        "LEFT JOIN contractors c ON p.client_id = c.id "
        "WHERE (p.name LIKE :query OR p.code LIKE :query OR p.address LIKE :query) ";
    
    QVariantMap params;
    params[":query"] = "%" + query + "%";
    
    if (!companyId.isEmpty()) {
        sql += "AND p.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY p.created_at DESC";
    
    QSqlQuery sqlQuery = Database::instance()->execute(sql, params);
    
    while (sqlQuery.next()) {
        Project *project = new Project();
        project->loadFromRecord(sqlQuery.record());
        project->setClientName(sqlQuery.value("client_name").toString());
        projects.append(project);
    }
    
    return projects;
}

int ProjectRepository::count(const QString &companyId)
{
    QString sql = "SELECT COUNT(*) FROM projects";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " WHERE company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryInt(sql, params);
}

int ProjectRepository::countByStatus(int status, const QString &companyId)
{
    QString sql = "SELECT COUNT(*) FROM projects WHERE status = :status";
    QVariantMap params;
    params[":status"] = status;
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryInt(sql, params);
}

double ProjectRepository::totalBudget(const QString &companyId)
{
    QString sql = "SELECT COALESCE(SUM(budget), 0) FROM projects";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " WHERE company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryDouble(sql, params);
}

double ProjectRepository::totalSpent(const QString &companyId)
{
    QString sql = "SELECT COALESCE(SUM(budget_spent), 0) FROM projects";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " WHERE company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryDouble(sql, params);
}

QVariantMap ProjectRepository::getStatistics(const QString &companyId)
{
    QVariantMap stats;
    
    stats["total"] = count(companyId);
    stats["planning"] = countByStatus(0, companyId);
    stats["active"] = countByStatus(1, companyId);
    stats["onHold"] = countByStatus(2, companyId);
    stats["completed"] = countByStatus(3, companyId);
    stats["cancelled"] = countByStatus(4, companyId);
    stats["totalBudget"] = totalBudget(companyId);
    stats["totalSpent"] = totalSpent(companyId);
    
    double budget = stats["totalBudget"].toDouble();
    double spent = stats["totalSpent"].toDouble();
    stats["totalRemaining"] = budget - spent;
    stats["utilization"] = budget > 0 ? (spent / budget * 100.0) : 0.0;
    
    return stats;
}

QVariantList ProjectRepository::getBudgetByMonth(int months, const QString &companyId)
{
    QVariantList result;
    
    QString sql = R"(
        SELECT 
            strftime('%Y-%m', created_at) as month,
            SUM(budget) as budget,
            SUM(budget_spent) as spent
        FROM projects
        WHERE created_at >= date('now', :months)
    )";
    
    QVariantMap params;
    params[":months"] = QString("-%1 months").arg(months);
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    sql += " GROUP BY month ORDER BY month";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        QVariantMap item;
        item["month"] = query.value("month").toString();
        item["budget"] = query.value("budget").toDouble();
        item["spent"] = query.value("spent").toDouble();
        result.append(item);
    }
    
    return result;
}

bool ProjectRepository::updateBudgetSpent(const QString &projectId, double amount)
{
    QSqlQuery query;
    query.prepare("UPDATE projects SET budget_spent = budget_spent + :amount WHERE id = :id");
    query.bindValue(":amount", amount);
    query.bindValue(":id", projectId);
    
    if (!query.exec()) {
        emit error(query.lastError().text());
        return false;
    }
    
    emit projectSaved(projectId);
    return true;
}

bool ProjectRepository::updateProgress(const QString &projectId, double progress)
{
    QSqlQuery query;
    query.prepare("UPDATE projects SET progress = :progress WHERE id = :id");
    query.bindValue(":progress", progress);
    query.bindValue(":id", projectId);
    
    if (!query.exec()) {
        emit error(query.lastError().text());
        return false;
    }
    
    emit projectSaved(projectId);
    return true;
}
