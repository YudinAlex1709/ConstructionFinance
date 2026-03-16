#ifndef FINANCEMANAGER_H
#define FINANCEMANAGER_H

#include <QObject>
#include <QDate>
#include <QVariantList>

class Transaction;

class FinanceManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(double totalBalance READ totalBalance NOTIFY balanceChanged)
    Q_PROPERTY(double monthlyIncome READ monthlyIncome NOTIFY statisticsChanged)
    Q_PROPERTY(double monthlyExpense READ monthlyExpense NOTIFY statisticsChanged)
    Q_PROPERTY(double quarterlyIncome READ quarterlyIncome NOTIFY statisticsChanged)
    Q_PROPERTY(double quarterlyExpense READ quarterlyExpense NOTIFY statisticsChanged)
    Q_PROPERTY(QVariantList recentTransactions READ recentTransactions NOTIFY transactionsChanged)

public:
    static FinanceManager* instance();
    
    double totalBalance() const { return m_totalBalance; }
    double monthlyIncome() const { return m_monthlyIncome; }
    double monthlyExpense() const { return m_monthlyExpense; }
    double quarterlyIncome() const { return m_quarterlyIncome; }
    double quarterlyExpense() const { return m_quarterlyExpense; }
    QVariantList recentTransactions() const { return m_recentTransactions; }
    
    Q_INVOKABLE void loadStatistics();
    Q_INVOKABLE void loadRecentTransactions(int count = 10);
    
    Q_INVOKABLE Transaction* createTransaction(const QVariantMap &data);
    Q_INVOKABLE bool updateTransaction(const QString &id, const QVariantMap &data);
    Q_INVOKABLE bool deleteTransaction(const QString &id);
    
    Q_INVOKABLE QVariantList getTransactions(int limit = 100);
    Q_INVOKABLE QVariantList getTransactionsByDateRange(const QDate &from, const QDate &to);
    Q_INVOKABLE QVariantList getTransactionsByProject(const QString &projectId);
    
    Q_INVOKABLE QVariantList getCashFlowData(int days = 30);
    Q_INVOKABLE QVariantList getExpensesByCategory(const QDate &from, const QDate &to);
    Q_INVOKABLE QVariantList getIncomeBySource(const QDate &from, const QDate &to);
    Q_INVOKABLE QVariantMap getProjectFinanceSummary(const QString &projectId);
    
    Q_INVOKABLE double getBudgetUtilization(const QString &projectId);

signals:
    void balanceChanged();
    void statisticsChanged();
    void transactionsChanged();
    void transactionCreated(const QString &id);
    void transactionUpdated(const QString &id);
    void transactionDeleted(const QString &id);
    void error(const QString &message);

private:
    explicit FinanceManager(QObject *parent = nullptr);
    void recalculateTotals();
    
    double m_totalBalance = 0.0;
    double m_monthlyIncome = 0.0;
    double m_monthlyExpense = 0.0;
    double m_quarterlyIncome = 0.0;
    double m_quarterlyExpense = 0.0;
    QVariantList m_recentTransactions;
    
    static FinanceManager *s_instance;
};

#endif // FINANCEMANAGER_H
