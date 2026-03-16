#include "transactionrepository.h"
#include "../../models/transaction.h"
#include "../database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

TransactionRepository* TransactionRepository::s_instance = nullptr;

TransactionRepository* TransactionRepository::instance()
{
    if (!s_instance) {
        s_instance = new TransactionRepository();
    }
    return s_instance;
}

TransactionRepository::TransactionRepository(QObject *parent)
    : QObject(parent)
{
}

Transaction* TransactionRepository::getById(const QString &id)
{
    QSqlQuery query = Database::instance()->execute(
        "SELECT t.*, p.name as project_name, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN projects p ON t.project_id = p.id "
        "LEFT JOIN contractors c ON t.contractor_id = c.id "
        "WHERE t.id = :id",
        {{":id", id}}
    );
    
    if (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setProjectName(query.value("project_name").toString());
        transaction->setContractorName(query.value("contractor_name").toString());
        return transaction;
    }
    
    return nullptr;
}

QList<Transaction*> TransactionRepository::getAll(int limit, const QString &companyId)
{
    QList<Transaction*> transactions;
    
    QString sql = 
        "SELECT t.*, p.name as project_name, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN projects p ON t.project_id = p.id "
        "LEFT JOIN contractors c ON t.contractor_id = c.id ";
    
    QVariantMap params;
    if (!companyId.isEmpty()) {
        sql += "WHERE t.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY t.date DESC, t.created_at DESC LIMIT :limit";
    params[":limit"] = limit;
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setProjectName(query.value("project_name").toString());
        transaction->setContractorName(query.value("contractor_name").toString());
        transactions.append(transaction);
    }
    
    return transactions;
}

QList<Transaction*> TransactionRepository::getByProject(const QString &projectId, int limit)
{
    QList<Transaction*> transactions;
    
    QSqlQuery query = Database::instance()->execute(
        "SELECT t.*, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN contractors c ON t.contractor_id = c.id "
        "WHERE t.project_id = :project_id "
        "ORDER BY t.date DESC LIMIT :limit",
        {{":project_id", projectId}, {":limit", limit}}
    );
    
    while (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setContractorName(query.value("contractor_name").toString());
        transactions.append(transaction);
    }
    
    return transactions;
}

QList<Transaction*> TransactionRepository::getByDateRange(const QDate &from, const QDate &to, const QString &companyId)
{
    QList<Transaction*> transactions;
    
    QString sql = 
        "SELECT t.*, p.name as project_name, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN projects p ON t.project_id = p.id "
        "LEFT JOIN contractors c ON t.contractor_id = c.id "
        "WHERE t.date BETWEEN :from AND :to ";
    
    QVariantMap params;
    params[":from"] = from;
    params[":to"] = to;
    
    if (!companyId.isEmpty()) {
        sql += "AND t.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY t.date DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setProjectName(query.value("project_name").toString());
        transaction->setContractorName(query.value("contractor_name").toString());
        transactions.append(transaction);
    }
    
    return transactions;
}

QList<Transaction*> TransactionRepository::getByType(int type, const QString &companyId)
{
    QList<Transaction*> transactions;
    
    QString sql = 
        "SELECT t.*, p.name as project_name, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN projects p ON t.project_id = p.id "
        "LEFT JOIN contractors c ON t.contractor_id = c.id "
        "WHERE t.type = :type ";
    
    QVariantMap params;
    params[":type"] = type;
    
    if (!companyId.isEmpty()) {
        sql += "AND t.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY t.date DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setProjectName(query.value("project_name").toString());
        transaction->setContractorName(query.value("contractor_name").toString());
        transactions.append(transaction);
    }
    
    return transactions;
}

QList<Transaction*> TransactionRepository::getRecent(int count, const QString &companyId)
{
    QList<Transaction*> transactions;
    
    QString sql = 
        "SELECT t.*, p.name as project_name, c.name as contractor_name "
        "FROM transactions t "
        "LEFT JOIN projects p ON t.project_id = p.id "
        "LEFT JOIN contractors c ON t.contractor_id = c.id ";
    
    QVariantMap params;
    if (!companyId.isEmpty()) {
        sql += "WHERE t.company_id = :company_id ";
        params[":company_id"] = companyId;
    }
    sql += "ORDER BY t.created_at DESC LIMIT :limit";
    params[":limit"] = count;
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        Transaction *transaction = new Transaction();
        transaction->loadFromRecord(query.record());
        transaction->setProjectName(query.value("project_name").toString());
        transaction->setContractorName(query.value("contractor_name").toString());
        transactions.append(transaction);
    }
    
    return transactions;
}

double TransactionRepository::getTotalIncome(const QDate &from, const QDate &to, const QString &companyId)
{
    QString sql = 
        "SELECT COALESCE(SUM(amount), 0) FROM transactions "
        "WHERE type IN (0, 4) AND status = 1 AND date BETWEEN :from AND :to ";  // Income, LoanReceived
    
    QVariantMap params;
    params[":from"] = from;
    params[":to"] = to;
    
    if (!companyId.isEmpty()) {
        sql += "AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryDouble(sql, params);
}

double TransactionRepository::getTotalExpense(const QDate &from, const QDate &to, const QString &companyId)
{
    QString sql = 
        "SELECT COALESCE(SUM(amount), 0) FROM transactions "
        "WHERE type IN (1, 3) AND status = 1 AND date BETWEEN :from AND :to ";  // Expense, LoanGiven
    
    QVariantMap params;
    params[":from"] = from;
    params[":to"] = to;
    
    if (!companyId.isEmpty()) {
        sql += "AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryDouble(sql, params);
}

double TransactionRepository::getBalance(const QString &companyId)
{
    QString sql = 
        "SELECT COALESCE(SUM(CASE WHEN type IN (0, 4) THEN amount ELSE -amount END), 0) "
        "FROM transactions WHERE status = 1";
    
    QVariantMap params;
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryDouble(sql, params);
}

QVariantList TransactionRepository::getByCategory(const QDate &from, const QDate &to, const QString &companyId)
{
    QVariantList result;
    
    QString sql = R"(
        SELECT 
            category,
            SUM(amount) as total,
            COUNT(*) as count
        FROM transactions
        WHERE type = 1 AND status = 1 AND date BETWEEN :from AND :to
    )";
    
    QVariantMap params;
    params[":from"] = from;
    params[":to"] = to;
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    sql += " GROUP BY category ORDER BY total DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        QVariantMap item;
        item["category"] = query.value("category").toString();
        item["total"] = query.value("total").toDouble();
        item["count"] = query.value("count").toInt();
        result.append(item);
    }
    
    return result;
}

QVariantList TransactionRepository::getByMonth(int months, const QString &companyId)
{
    QVariantList result;
    
    QString sql = R"(
        SELECT 
            strftime('%Y-%m', date) as month,
            SUM(CASE WHEN type IN (0, 4) THEN amount ELSE 0 END) as income,
            SUM(CASE WHEN type IN (1, 3) THEN amount ELSE 0 END) as expense
        FROM transactions
        WHERE status = 1 AND date >= date('now', :months)
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
        item["income"] = query.value("income").toDouble();
        item["expense"] = query.value("expense").toDouble();
        result.append(item);
    }
    
    return result;
}

QVariantList TransactionRepository::getCashFlow(int days, const QString &companyId)
{
    QVariantList result;
    
    QString sql = R"(
        SELECT 
            date,
            SUM(CASE WHEN type IN (0, 4) THEN amount ELSE 0 END) as income,
            SUM(CASE WHEN type IN (1, 3) THEN amount ELSE 0 END) as expense
        FROM transactions
        WHERE status = 1 AND date >= date('now', :days)
    )";
    
    QVariantMap params;
    params[":days"] = QString("-%1 days").arg(days);
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    sql += " GROUP BY date ORDER BY date";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    double runningBalance = 0;
    while (query.next()) {
        QVariantMap item;
        QDate date = query.value("date").toDate();
        double income = query.value("income").toDouble();
        double expense = query.value("expense").toDouble();
        runningBalance += income - expense;
        
        item["date"] = date;
        item["income"] = income;
        item["expense"] = expense;
        item["balance"] = runningBalance;
        result.append(item);
    }
    
    return result;
}

double TransactionRepository::getProjectIncome(const QString &projectId)
{
    return Database::instance()->queryDouble(
        "SELECT COALESCE(SUM(amount), 0) FROM transactions "
        "WHERE project_id = :project_id AND type IN (0, 4) AND status = 1",
        {{":project_id", projectId}}
    );
}

double TransactionRepository::getProjectExpense(const QString &projectId)
{
    return Database::instance()->queryDouble(
        "SELECT COALESCE(SUM(amount), 0) FROM transactions "
        "WHERE project_id = :project_id AND type IN (1, 3) AND status = 1",
        {{":project_id", projectId}}
    );
}
