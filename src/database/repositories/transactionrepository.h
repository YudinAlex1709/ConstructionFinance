#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QObject>
#include <QList>
#include <QDate>
#include <QVariantMap>

class Transaction;

class TransactionRepository : public QObject
{
    Q_OBJECT

public:
    static TransactionRepository* instance();
    
    // CRUD operations
    Q_INVOKABLE Transaction* getById(const QString &id);
    Q_INVOKABLE QList<Transaction*> getAll(int limit = 100, const QString &companyId = QString());
    Q_INVOKABLE QList<Transaction*> getByProject(const QString &projectId, int limit = 100);
    Q_INVOKABLE QList<Transaction*> getByDateRange(const QDate &from, const QDate &to, const QString &companyId = QString());
    Q_INVOKABLE QList<Transaction*> getByType(int type, const QString &companyId = QString());
    Q_INVOKABLE QList<Transaction*> getRecent(int count = 10, const QString &companyId = QString());
    
    // Statistics
    Q_INVOKABLE double getTotalIncome(const QDate &from, const QDate &to, const QString &companyId = QString());
    Q_INVOKABLE double getTotalExpense(const QDate &from, const QDate &to, const QString &companyId = QString());
    Q_INVOKABLE double getBalance(const QString &companyId = QString());
    
    // Aggregations
    Q_INVOKABLE QVariantList getByCategory(const QDate &from, const QDate &to, const QString &companyId = QString());
    Q_INVOKABLE QVariantList getByMonth(int months = 12, const QString &companyId = QString());
    Q_INVOKABLE QVariantList getCashFlow(int days = 30, const QString &companyId = QString());
    
    // Project finance
    Q_INVOKABLE double getProjectIncome(const QString &projectId);
    Q_INVOKABLE double getProjectExpense(const QString &projectId);

signals:
    void transactionSaved(const QString &id);
    void transactionRemoved(const QString &id);
    void error(const QString &message);

private:
    explicit TransactionRepository(QObject *parent = nullptr);
    static TransactionRepository *s_instance;
};

#endif // TRANSACTIONREPOSITORY_H
