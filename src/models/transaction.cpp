#include "transaction.h"
#include "../database/database.h"
#include "../utils/currencyutils.h"
#include "../utils/dateutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Transaction::Transaction(QObject *parent)
    : BaseModel(parent)
{
}

void Transaction::setType(TransactionType type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
        setIsDirty(true);
    }
}

void Transaction::setCategory(const QString &cat)
{
    if (m_category != cat) {
        m_category = cat;
        emit categoryChanged();
        setIsDirty(true);
    }
}

void Transaction::setAmount(double amount)
{
    amount = qMax(0.0, amount);
    if (!qFuzzyCompare(m_amount, amount)) {
        m_amount = amount;
        emit amountChanged();
        setIsDirty(true);
    }
}

void Transaction::setCurrency(const QString &curr)
{
    if (m_currency != curr) {
        m_currency = curr;
        emit currencyChanged();
        setIsDirty(true);
    }
}

void Transaction::setDate(const QDate &date)
{
    if (m_date != date) {
        m_date = date;
        emit dateChanged();
        setIsDirty(true);
    }
}

void Transaction::setProjectId(const QString &id)
{
    if (m_projectId != id) {
        m_projectId = id;
        emit projectIdChanged();
        setIsDirty(true);
    }
}

void Transaction::setProjectName(const QString &name)
{
    if (m_projectName != name) {
        m_projectName = name;
        emit projectNameChanged();
    }
}

void Transaction::setContractorId(const QString &id)
{
    if (m_contractorId != id) {
        m_contractorId = id;
        emit contractorIdChanged();
        setIsDirty(true);
    }
}

void Transaction::setContractorName(const QString &name)
{
    if (m_contractorName != name) {
        m_contractorName = name;
        emit contractorNameChanged();
    }
}

void Transaction::setBankAccountId(const QString &id)
{
    if (m_bankAccountId != id) {
        m_bankAccountId = id;
        emit bankAccountIdChanged();
        setIsDirty(true);
    }
}

void Transaction::setDescription(const QString &desc)
{
    if (m_description != desc) {
        m_description = desc;
        emit descriptionChanged();
        setIsDirty(true);
    }
}

void Transaction::setDocumentNumber(const QString &num)
{
    if (m_documentNumber != num) {
        m_documentNumber = num;
        emit documentNumberChanged();
        setIsDirty(true);
    }
}

void Transaction::setDocumentDate(const QDate &date)
{
    if (m_documentDate != date) {
        m_documentDate = date;
        emit documentDateChanged();
        setIsDirty(true);
    }
}

void Transaction::setStatus(TransactionStatus status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged();
        setIsDirty(true);
    }
}

void Transaction::setCreatedById(const QString &id)
{
    if (m_createdById != id) {
        m_createdById = id;
        emit createdByIdChanged();
        setIsDirty(true);
    }
}

QString Transaction::typeLabel() const
{
    switch (m_type) {
        case Income: return tr("Доход");
        case Expense: return tr("Расход");
        case Transfer: return tr("Перевод");
        case LoanGiven: return tr("Займ выдан");
        case LoanReceived: return tr("Займ получен");
        default: return tr("Неизвестно");
    }
}

QColor Transaction::typeColor() const
{
    switch (m_type) {
        case Income:
        case LoanReceived:
            return QColor("#10B981"); // green
        case Expense:
        case LoanGiven:
            return QColor("#EF4444"); // red
        case Transfer:
            return QColor("#3B82F6"); // blue
        default:
            return QColor("#6B7280"); // gray
    }
}

QString Transaction::formattedAmount() const
{
    QString prefix = isIncome() ? "+" : (isExpense() ? "-" : "");
    return prefix + CurrencyUtils::format(m_amount, m_currency);
}

QString Transaction::formattedDate() const
{
    return DateUtils::formatShort(m_date);
}

QString Transaction::statusLabel() const
{
    switch (m_status) {
        case Pending: return tr("Ожидает");
        case Confirmed: return tr("Подтверждена");
        case Cancelled: return tr("Отменена");
        default: return tr("Неизвестно");
    }
}

bool Transaction::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    if (!m_date.isValid()) {
        m_date = QDate::currentDate();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO transactions 
        (id, type, category, amount, currency, date, project_id, company_id,
         contractor_id, bank_account_id, description, document_number, document_date,
         created_by_id, status, created_at)
        VALUES 
        (:id, :type, :category, :amount, :currency, :date, :project_id, :company_id,
         :contractor_id, :bank_account_id, :description, :document_number, :document_date,
         :created_by_id, :status, :created_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":type", static_cast<int>(m_type));
    query.bindValue(":category", m_category);
    query.bindValue(":amount", m_amount);
    query.bindValue(":currency", m_currency);
    query.bindValue(":date", m_date);
    query.bindValue(":project_id", m_projectId);
    query.bindValue(":company_id", "default"); // TODO: get from current user
    query.bindValue(":contractor_id", m_contractorId);
    query.bindValue(":bank_account_id", m_bankAccountId);
    query.bindValue(":description", m_description);
    query.bindValue(":document_number", m_documentNumber);
    query.bindValue(":document_date", m_documentDate);
    query.bindValue(":created_by_id", m_createdById);
    query.bindValue(":status", static_cast<int>(m_status));
    query.bindValue(":created_at", m_createdAt);
    
    if (!query.exec()) {
        qWarning() << "Failed to save transaction:" << query.lastError().text();
        emit error(tr("Ошибка сохранения операции: %1").arg(query.lastError().text()));
        return false;
    }
    
    // Update project budget spent if it's an expense
    if (!m_projectId.isEmpty() && m_status == Confirmed) {
        if (m_type == Expense) {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE projects SET budget_spent = budget_spent + :amount WHERE id = :project_id");
            updateQuery.bindValue(":amount", m_amount);
            updateQuery.bindValue(":project_id", m_projectId);
            updateQuery.exec();
        }
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool Transaction::remove()
{
    if (m_id.isEmpty()) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM transactions WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove transaction:" << query.lastError().text();
        emit error(tr("Ошибка удаления операции: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void Transaction::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setType(static_cast<TransactionType>(record.value("type").toInt()));
    setCategory(record.value("category").toString());
    setAmount(record.value("amount").toDouble());
    setCurrency(record.value("currency").toString());
    setDate(record.value("date").toDate());
    setProjectId(record.value("project_id").toString());
    setContractorId(record.value("contractor_id").toString());
    setBankAccountId(record.value("bank_account_id").toString());
    setDescription(record.value("description").toString());
    setDocumentNumber(record.value("document_number").toString());
    setDocumentDate(record.value("document_date").toDate());
    setCreatedById(record.value("created_by_id").toString());
    setStatus(static_cast<TransactionStatus>(record.value("status").toInt()));
    setCreatedAt(record.value("created_at").toDateTime());
    setIsDirty(false);
}

QVariantMap Transaction::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["type"] = static_cast<int>(m_type);
    map["typeLabel"] = typeLabel();
    map["category"] = m_category;
    map["amount"] = m_amount;
    map["formattedAmount"] = formattedAmount();
    map["currency"] = m_currency;
    map["date"] = m_date;
    map["formattedDate"] = formattedDate();
    map["projectId"] = m_projectId;
    map["projectName"] = m_projectName;
    map["contractorId"] = m_contractorId;
    map["contractorName"] = m_contractorName;
    map["bankAccountId"] = m_bankAccountId;
    map["description"] = m_description;
    map["documentNumber"] = m_documentNumber;
    map["documentDate"] = m_documentDate;
    map["status"] = static_cast<int>(m_status);
    map["statusLabel"] = statusLabel();
    map["isIncome"] = isIncome();
    map["isExpense"] = isExpense();
    return map;
}

void Transaction::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("type")) setType(static_cast<TransactionType>(map["type"].toInt()));
    if (map.contains("category")) setCategory(map["category"].toString());
    if (map.contains("amount")) setAmount(map["amount"].toDouble());
    if (map.contains("currency")) setCurrency(map["currency"].toString());
    if (map.contains("date")) setDate(map["date"].toDate());
    if (map.contains("projectId")) setProjectId(map["projectId"].toString());
    if (map.contains("contractorId")) setContractorId(map["contractorId"].toString());
    if (map.contains("bankAccountId")) setBankAccountId(map["bankAccountId"].toString());
    if (map.contains("description")) setDescription(map["description"].toString());
    if (map.contains("documentNumber")) setDocumentNumber(map["documentNumber"].toString());
    if (map.contains("documentDate")) setDocumentDate(map["documentDate"].toDate());
    if (map.contains("status")) setStatus(static_cast<TransactionStatus>(map["status"].toInt()));
    if (map.contains("createdById")) setCreatedById(map["createdById"].toString());
}
