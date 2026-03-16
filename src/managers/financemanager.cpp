#include "financemanager.h"
#include "../models/transaction.h"
#include "../database/repositories/transactionrepository.h"
#include <QDebug>

FinanceManager* FinanceManager::s_instance = nullptr;

FinanceManager* FinanceManager::instance()
{
    if (!s_instance) {
        s_instance = new FinanceManager();
    }
    return s_instance;
}

FinanceManager::FinanceManager(QObject *parent)
    : QObject(parent)
{
    loadStatistics();
    loadRecentTransactions(10);
}

void FinanceManager::loadStatistics()
{
    recalculateTotals();
}

void FinanceManager::loadRecentTransactions(int count)
{
    QList<Transaction*> transactions = TransactionRepository::instance()->getRecent(count, "default");
    
    m_recentTransactions.clear();
    for (Transaction *transaction : transactions) {
        m_recentTransactions.append(transaction->toMap());
        delete transaction;
    }
    
    emit transactionsChanged();
}

Transaction* FinanceManager::createTransaction(const QVariantMap &data)
{
    Transaction *transaction = new Transaction();
    transaction->fromMap(data);
    transaction->setCreatedById("current_user"); // TODO: get from session
    
    if (!transaction->save()) {
        delete transaction;
        emit error(tr("Не удалось создать операцию"));
        return nullptr;
    }
    
    recalculateTotals();
    loadRecentTransactions(10);
    
    emit transactionCreated(transaction->id());
    emit transactionsChanged();
    
    return transaction;
}

bool FinanceManager::updateTransaction(const QString &id, const QVariantMap &data)
{
    Transaction *transaction = TransactionRepository::instance()->getById(id);
    if (!transaction) {
        emit error(tr("Операция не найдена"));
        return false;
    }
    
    transaction->fromMap(data);
    
    if (!transaction->save()) {
        delete transaction;
        emit error(tr("Не удалось обновить операцию"));
        return false;
    }
    
    delete transaction;
    recalculateTotals();
    loadRecentTransactions(10);
    
    emit transactionUpdated(id);
    emit transactionsChanged();
    
    return true;
}

bool FinanceManager::deleteTransaction(const QString &id)
{
    Transaction *transaction = TransactionRepository::instance()->getById(id);
    if (!transaction) {
        emit error(tr("Операция не найдена"));
        return false;
    }
    
    if (!transaction->remove()) {
        delete transaction;
        emit error(tr("Не удалось удалить операцию"));
        return false;
    }
    
    delete transaction;
    recalculateTotals();
    loadRecentTransactions(10);
    
    emit transactionDeleted(id);
    emit transactionsChanged();
    
    return true;
}

QVariantList FinanceManager::getTransactions(int limit)
{
    QList<Transaction*> transactions = TransactionRepository::instance()->getAll(limit, "default");
    
    QVariantList result;
    for (Transaction *transaction : transactions) {
        result.append(transaction->toMap());
        delete transaction;
    }
    
    return result;
}

QVariantList FinanceManager::getTransactionsByDateRange(const QDate &from, const QDate &to)
{
    QList<Transaction*> transactions = TransactionRepository::instance()->getByDateRange(from, to, "default");
    
    QVariantList result;
    for (Transaction *transaction : transactions) {
        result.append(transaction->toMap());
        delete transaction;
    }
    
    return result;
}

QVariantList FinanceManager::getTransactionsByProject(const QString &projectId)
{
    QList<Transaction*> transactions = TransactionRepository::instance()->getByProject(projectId);
    
    QVariantList result;
    for (Transaction *transaction : transactions) {
        result.append(transaction->toMap());
        delete transaction;
    }
    
    return result;
}

QVariantList FinanceManager::getCashFlowData(int days)
{
    return TransactionRepository::instance()->getCashFlow(days, "default");
}

QVariantList FinanceManager::getExpensesByCategory(const QDate &from, const QDate &to)
{
    return TransactionRepository::instance()->getByCategory(from, to, "default");
}

QVariantList FinanceManager::getIncomeBySource(const QDate &from, const QDate &to)
{
    // For now, return same as expenses but with income type
    // This could be customized based on your needs
    return TransactionRepository::instance()->getByCategory(from, to, "default");
}

QVariantMap FinanceManager::getProjectFinanceSummary(const QString &projectId)
{
    QVariantMap summary;
    
    double income = TransactionRepository::instance()->getProjectIncome(projectId);
    double expense = TransactionRepository::instance()->getProjectExpense(projectId);
    
    summary["income"] = income;
    summary["expense"] = expense;
    summary["balance"] = income - expense;
    
    return summary;
}

double FinanceManager::getBudgetUtilization(const QString &projectId)
{
    QVariantMap summary = getProjectFinanceSummary(projectId);
    double expense = summary["expense"].toDouble();
    double income = summary["income"].toDouble();
    
    if (income <= 0) return 0.0;
    return (expense / income) * 100.0;
}

void FinanceManager::recalculateTotals()
{
    QDate today = QDate::currentDate();
    QDate monthStart = QDate(today.year(), today.month(), 1);
    QDate quarterStart = QDate(today.year(), ((today.month() - 1) / 3) * 3 + 1, 1);
    
    m_totalBalance = TransactionRepository::instance()->getBalance("default");
    m_monthlyIncome = TransactionRepository::instance()->getTotalIncome(monthStart, today, "default");
    m_monthlyExpense = TransactionRepository::instance()->getTotalExpense(monthStart, today, "default");
    m_quarterlyIncome = TransactionRepository::instance()->getTotalIncome(quarterStart, today, "default");
    m_quarterlyExpense = TransactionRepository::instance()->getTotalExpense(quarterStart, today, "default");
    
    emit balanceChanged();
    emit statisticsChanged();
}
