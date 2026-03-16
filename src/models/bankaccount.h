#ifndef BANKACCOUNT_H
#define BANKACCOUNT_H

#include "basemodel.h"

class BankAccount : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QString companyId READ companyId WRITE setCompanyId NOTIFY companyIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString bankName READ bankName WRITE setBankName NOTIFY bankNameChanged)
    Q_PROPERTY(QString accountNumber READ accountNumber WRITE setAccountNumber NOTIFY accountNumberChanged)
    Q_PROPERTY(QString bik READ bik WRITE setBik NOTIFY bikChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)
    Q_PROPERTY(double balance READ balance WRITE setBalance NOTIFY balanceChanged)
    Q_PROPERTY(bool isDefault READ isDefault WRITE setIsDefault NOTIFY isDefaultChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
    Q_PROPERTY(QString maskedNumber READ maskedNumber NOTIFY accountNumberChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY nameChanged)

public:
    explicit BankAccount(QObject *parent = nullptr);
    
    // Getters
    QString companyId() const { return m_companyId; }
    QString name() const { return m_name; }
    QString bankName() const { return m_bankName; }
    QString accountNumber() const { return m_accountNumber; }
    QString bik() const { return m_bik; }
    QString currency() const { return m_currency; }
    double balance() const { return m_balance; }
    bool isDefault() const { return m_isDefault; }
    bool isActive() const { return m_isActive; }
    
    // Setters
    void setCompanyId(const QString &id);
    void setName(const QString &name);
    void setBankName(const QString &name);
    void setAccountNumber(const QString &number);
    void setBik(const QString &bik);
    void setCurrency(const QString &curr);
    void setBalance(double balance);
    void setIsDefault(bool isDefault);
    void setIsActive(bool active);
    
    // Helper methods
    QString maskedNumber() const;
    QString displayName() const;
    Q_INVOKABLE QString formattedBalance() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void companyIdChanged();
    void nameChanged();
    void bankNameChanged();
    void accountNumberChanged();
    void bikChanged();
    void currencyChanged();
    void balanceChanged();
    void isDefaultChanged();
    void isActiveChanged();

private:
    QString m_companyId;
    QString m_name;
    QString m_bankName;
    QString m_accountNumber;
    QString m_bik;
    QString m_currency = "RUB";
    double m_balance = 0.0;
    bool m_isDefault = false;
    bool m_isActive = true;
};

#endif // BANKACCOUNT_H
