#include "project.h"
#include "../database/database.h"
#include "../utils/currencyutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Project::Project(QObject *parent)
    : BaseModel(parent)
{
}

void Project::setCode(const QString &code)
{
    if (m_code != code) {
        m_code = code;
        emit codeChanged();
        setIsDirty(true);
    }
}

void Project::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        setIsDirty(true);
    }
}

void Project::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged();
        setIsDirty(true);
    }
}

void Project::setAddress(const QString &addr)
{
    if (m_address != addr) {
        m_address = addr;
        emit addressChanged();
        setIsDirty(true);
    }
}

void Project::setCity(const QString &city)
{
    if (m_city != city) {
        m_city = city;
        emit cityChanged();
        setIsDirty(true);
    }
}

void Project::setStatus(ProjectStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
        setIsDirty(true);
    }
}

void Project::setStartDate(const QDate &date)
{
    if (m_startDate != date) {
        m_startDate = date;
        emit startDateChanged();
        setIsDirty(true);
    }
}

void Project::setEndDatePlanned(const QDate &date)
{
    if (m_endDatePlanned != date) {
        m_endDatePlanned = date;
        emit endDatePlannedChanged();
        setIsDirty(true);
    }
}

void Project::setEndDateActual(const QDate &date)
{
    if (m_endDateActual != date) {
        m_endDateActual = date;
        emit endDateActualChanged();
        setIsDirty(true);
    }
}

void Project::setBudget(double budget)
{
    if (!qFuzzyCompare(m_budget, budget)) {
        m_budget = budget;
        emit budgetChanged();
        emit budgetRemainingChanged();
        setIsDirty(true);
    }
}

void Project::setBudgetSpent(double spent)
{
    if (!qFuzzyCompare(m_budgetSpent, spent)) {
        m_budgetSpent = spent;
        emit budgetSpentChanged();
        emit budgetRemainingChanged();
        setIsDirty(true);
    }
}

void Project::setProgress(double progress)
{
    progress = qBound(0.0, progress, 100.0);
    if (!qFuzzyCompare(m_progress, progress)) {
        m_progress = progress;
        emit progressChanged();
        setIsDirty(true);
    }
}

void Project::setClientId(const QString &id)
{
    if (m_clientId != id) {
        m_clientId = id;
        emit clientIdChanged();
        setIsDirty(true);
    }
}

void Project::setClientName(const QString &name)
{
    if (m_clientName != name) {
        m_clientName = name;
        emit clientNameChanged();
    }
}

void Project::setCompanyId(const QString &id)
{
    if (m_companyId != id) {
        m_companyId = id;
        emit companyIdChanged();
        setIsDirty(true);
    }
}

QString Project::statusLabel() const
{
    switch (m_status) {
        case Planning: return tr("Планирование");
        case Active: return tr("Активный");
        case OnHold: return tr("Приостановлен");
        case Completed: return tr("Завершен");
        case Cancelled: return tr("Отменен");
        default: return tr("Неизвестно");
    }
}

QColor Project::statusColor() const
{
    switch (m_status) {
        case Planning: return QColor("#F59E0B");  // amber
        case Active: return QColor("#10B981");    // emerald
        case OnHold: return QColor("#6B7280");    // gray
        case Completed: return QColor("#3B82F6"); // blue
        case Cancelled: return QColor("#EF4444"); // red
        default: return QColor("#9CA3AF");
    }
}

QString Project::formattedBudget() const
{
    return CurrencyUtils::format(m_budget);
}

QString Project::formattedSpent() const
{
    return CurrencyUtils::format(m_budgetSpent);
}

QString Project::formattedRemaining() const
{
    return CurrencyUtils::format(budgetRemaining());
}

double Project::budgetUtilization() const
{
    if (m_budget <= 0) return 0.0;
    return (m_budgetSpent / m_budget) * 100.0;
}

bool Project::isOverBudget() const
{
    return m_budgetSpent > m_budget;
}

int Project::daysRemaining() const
{
    if (!m_endDatePlanned.isValid()) return -1;
    return QDate::currentDate().daysTo(m_endDatePlanned);
}

bool Project::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO projects 
        (id, code, name, description, address, city, status, start_date, 
         end_date_planned, end_date_actual, budget, budget_spent, progress,
         client_id, company_id, created_at, updated_at)
        VALUES 
        (:id, :code, :name, :description, :address, :city, :status, :start_date,
         :end_date_planned, :end_date_actual, :budget, :budget_spent, :progress,
         :client_id, :company_id, :created_at, :updated_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":code", m_code);
    query.bindValue(":name", m_name);
    query.bindValue(":description", m_description);
    query.bindValue(":address", m_address);
    query.bindValue(":city", m_city);
    query.bindValue(":status", static_cast<int>(m_status));
    query.bindValue(":start_date", m_startDate);
    query.bindValue(":end_date_planned", m_endDatePlanned);
    query.bindValue(":end_date_actual", m_endDateActual);
    query.bindValue(":budget", m_budget);
    query.bindValue(":budget_spent", m_budgetSpent);
    query.bindValue(":progress", m_progress);
    query.bindValue(":client_id", m_clientId);
    query.bindValue(":company_id", m_companyId);
    query.bindValue(":created_at", m_createdAt);
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    
    if (!query.exec()) {
        qWarning() << "Failed to save project:" << query.lastError().text();
        emit error(tr("Ошибка сохранения проекта: %1").arg(query.lastError().text()));
        return false;
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool Project::remove()
{
    if (m_id.isEmpty()) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM projects WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove project:" << query.lastError().text();
        emit error(tr("Ошибка удаления проекта: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void Project::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setCode(record.value("code").toString());
    setName(record.value("name").toString());
    setDescription(record.value("description").toString());
    setAddress(record.value("address").toString());
    setCity(record.value("city").toString());
    setStatus(static_cast<ProjectStatus>(record.value("status").toInt()));
    setStartDate(record.value("start_date").toDate());
    setEndDatePlanned(record.value("end_date_planned").toDate());
    setEndDateActual(record.value("end_date_actual").toDate());
    setBudget(record.value("budget").toDouble());
    setBudgetSpent(record.value("budget_spent").toDouble());
    setProgress(record.value("progress").toDouble());
    setClientId(record.value("client_id").toString());
    setCompanyId(record.value("company_id").toString());
    setCreatedAt(record.value("created_at").toDateTime());
    setUpdatedAt(record.value("updated_at").toDateTime());
    setIsDirty(false);
}

QVariantMap Project::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["code"] = m_code;
    map["name"] = m_name;
    map["description"] = m_description;
    map["address"] = m_address;
    map["city"] = m_city;
    map["status"] = static_cast<int>(m_status);
    map["statusLabel"] = statusLabel();
    map["startDate"] = m_startDate;
    map["endDatePlanned"] = m_endDatePlanned;
    map["endDateActual"] = m_endDateActual;
    map["budget"] = m_budget;
    map["budgetSpent"] = m_budgetSpent;
    map["budgetRemaining"] = budgetRemaining();
    map["progress"] = m_progress;
    map["clientId"] = m_clientId;
    map["clientName"] = m_clientName;
    map["companyId"] = m_companyId;
    return map;
}

void Project::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("code")) setCode(map["code"].toString());
    if (map.contains("name")) setName(map["name"].toString());
    if (map.contains("description")) setDescription(map["description"].toString());
    if (map.contains("address")) setAddress(map["address"].toString());
    if (map.contains("city")) setCity(map["city"].toString());
    if (map.contains("status")) setStatus(static_cast<ProjectStatus>(map["status"].toInt()));
    if (map.contains("startDate")) setStartDate(map["startDate"].toDate());
    if (map.contains("endDatePlanned")) setEndDatePlanned(map["endDatePlanned"].toDate());
    if (map.contains("endDateActual")) setEndDateActual(map["endDateActual"].toDate());
    if (map.contains("budget")) setBudget(map["budget"].toDouble());
    if (map.contains("budgetSpent")) setBudgetSpent(map["budgetSpent"].toDouble());
    if (map.contains("progress")) setProgress(map["progress"].toDouble());
    if (map.contains("clientId")) setClientId(map["clientId"].toString());
    if (map.contains("companyId")) setCompanyId(map["companyId"].toString());
}
