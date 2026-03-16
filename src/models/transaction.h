#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "basemodel.h"
#include <QDate>
#include <QColor>

class Transaction : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(TransactionType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString category READ category WRITE setCategory NOTIFY categoryChanged)
    Q_PROPERTY(double amount READ amount WRITE setAmount NOTIFY amountChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(QDate date READ date WRITE setDate NOTIFY dateChanged)
    Q_PROPERTY(QString projectId READ projectId WRITE setProjectId NOTIFY projectIdChanged)
    Q_PROPERTY(QString projectName READ projectName WRITE setProjectName NOTIFY projectNameChanged)
    Q_PROPERTY(QString contractorId READ contractorId WRITE setContractorId NOTIFY contractorIdChanged)
    Q_PROPERTY(QString contractorName READ contractorName WRITE setContractorName NOTIFY contractorNameChanged)
    Q_PROPERTY(QString bankAccountId READ bankAccountId WRITE setBankAccountId NOTIFY bankAccountIdChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(QString documentNumber READ documentNumber WRITE setDocumentNumber NOTIFY documentNumberChanged)
    Q_PROPERTY(QDate documentDate READ documentDate WRITE setDocumentDate NOTIFY documentDateChanged)
    Q_PROPERTY(TransactionStatus status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QString createdById READ createdById WRITE setCreatedById NOTIFY createdByIdChanged)
    Q_PROPERTY(QString typeLabel READ typeLabel NOTIFY typeChanged)
    Q_PROPERTY(QColor typeColor READ typeColor NOTIFY typeChanged)
    Q_PROPERTY(bool isIncome READ isIncome NOTIFY typeChanged)
    Q_PROPERTY(bool isExpense READ isExpense NOTIFY typeChanged)

public:
    enum TransactionType {
        Income = 0,         // Доход
        Expense = 1,        // Расход
        Transfer = 2,       // Перевод
        LoanGiven = 3,      // Займ выдан
        LoanReceived = 4    // Займ получен
    };
    Q_ENUM(TransactionType)

    enum TransactionStatus {
        Pending = 0,        // Ожидает подтверждения
        Confirmed = 1,      // Подтверждена
        Cancelled = 2       // Отменена
    };
    Q_ENUM(TransactionStatus)

    explicit Transaction(QObject *parent = nullptr);
    
    // Getters
    TransactionType type() const { return m_type; }
    QString category() const { return m_category; }
    double amount() const { return m_amount; }
    QString currency() const { return m_currency; }
    QDate date() const { return m_date; }
    QString projectId() const { return m_projectId; }
    QString projectName() const { return m_projectName; }
    QString contractorId() const { return m_contractorId; }
    QString contractorName() const { return m_contractorName; }
    QString bankAccountId() const { return m_bankAccountId; }
    QString description() const { return m_description; }
    QString documentNumber() const { return m_documentNumber; }
    QDate documentDate() const { return m_documentDate; }
    TransactionStatus status() const { return m_status; }
    QString createdById() const { return m_createdById; }
    
    // Setters
    void setType(TransactionType type);
    void setCategory(const QString &cat);
    void setAmount(double amount);
    void setCurrency(const QString &curr);
    void setDate(const QDate &date);
    void setProjectId(const QString &id);
    void setProjectName(const QString &name);
    void setContractorId(const QString &id);
    void setContractorName(const QString &name);
    void setBankAccountId(const QString &id);
    void setDescription(const QString &desc);
    void setDocumentNumber(const QString &num);
    void setDocumentDate(const QDate &date);
    void setStatus(TransactionStatus status);
    void setCreatedById(const QString &id);
    
    // Helper methods
    QString typeLabel() const;
    QColor typeColor() const;
    bool isIncome() const { return m_type == Income || m_type == LoanReceived; }
    bool isExpense() const { return m_type == Expense || m_type == LoanGiven; }
    
    Q_INVOKABLE QString formattedAmount() const;
    Q_INVOKABLE QString formattedDate() const;
    Q_INVOKABLE QString statusLabel() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void typeChanged();
    void categoryChanged();
    void amountChanged();
    void currencyChanged();
    void dateChanged();
    void projectIdChanged();
    void projectNameChanged();
    void contractorIdChanged();
    void contractorNameChanged();
    void bankAccountIdChanged();
    void descriptionChanged();
    void documentNumberChanged();
    void documentDateChanged();
    void statusChanged();
    void createdByIdChanged();

private:
    TransactionType m_type = Expense;
    QString m_category;
    double m_amount = 0.0;
    QString m_currency = "RUB";
    QDate m_date;
    QString m_projectId;
    QString m_projectName;
    QString m_contractorId;
    QString m_contractorName;
    QString m_bankAccountId;
    QString m_description;
    QString m_documentNumber;
    QDate m_documentDate;
    TransactionStatus m_status = Confirmed;
    QString m_createdById;
};

#endif // TRANSACTION_H
