#include "bankaccount.h"
#include "../database/database.h"
#include "../utils/currencyutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

BankAccount::BankAccount(QObject *parent)
    : BaseModel(parent)
{
}

void BankAccount::setCompanyId(const QString &id)
{
    if (m_companyId != id) {
        m_companyId = id;
        emit companyIdChanged();
        setIsDirty(true);
    }
}

void BankAccount::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        setIsDirty(true);
    }
}

void BankAccount::setBankName(const QString &name)
{
    if (m_bankName != name) {
        m_bankName = name;
        emit bankNameChanged();
        setIsDirty(true);
    }
}

void BankAccount::setAccountNumber(const QString &number)
{
    if (m_accountNumber != number) {
        m_accountNumber = number;
        emit accountNumberChanged();
        setIsDirty(true);
    }
}

void BankAccount::setBik(const QString &bik)
{
    if (m_bik != bik) {
        m_bik = bik;
        emit bikChanged();
        setIsDirty(true);
    }
}

void BankAccount::setCurrency(const QString &curr)
{
    if (m_currency != curr) {
        m_currency = curr;
        emit currencyChanged();
        setIsDirty(true);
    }
}

void BankAccount::setBalance(double balance)
{
    if (!qFuzzyCompare(m_balance, balance)) {
        m_balance = balance;
        emit balanceChanged();
        setIsDirty(true);
    }
}

void BankAccount::setIsDefault(bool isDefault)
{
    if (m_isDefault != isDefault) {
        m_isDefault = isDefault;
        emit isDefaultChanged();
        setIsDirty(true);
    }
}

void BankAccount::setIsActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        emit isActiveChanged();
        setIsDirty(true);
    }
}

QString BankAccount::maskedNumber() const
{
    if (m_accountNumber.length() <= 4) return m_accountNumber;
    return "****" + m_accountNumber.right(4);
}

QString BankAccount::displayName() const
{
    if (!m_name.isEmpty() && !m_bankName.isEmpty()) {
        return m_name + " (" + m_bankName + ")";
    }
    return m_name.isEmpty() ? m_bankName : m_name;
}

QString BankAccount::formattedBalance() const
{
    return CurrencyUtils::format(m_balance, m_currency);
}

bool BankAccount::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO bank_accounts 
        (id, company_id, name, bank_name, account_number, bik, currency, balance, is_default, is_active, created_at)
        VALUES 
        (:id, :company_id, :name, :bank_name, :account_number, :bik, :currency, :balance, :is_default, :is_active, :created_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":company_id", m_companyId);
    query.bindValue(":name", m_name);
    query.bindValue(":bank_name", m_bankName);
    query.bindValue(":account_number", m_accountNumber);
    query.bindValue(":bik", m_bik);
    query.bindValue(":currency", m_currency);
    query.bindValue(":balance", m_balance);
    query.bindValue(":is_default", m_isDefault ? 1 : 0);
    query.bindValue(":is_active", m_isActive ? 1 : 0);
    query.bindValue(":created_at", m_createdAt);
    
    if (!query.exec()) {
        qWarning() << "Failed to save bank account:" << query.lastError().text();
        emit error(tr("Ошибка сохранения счета: %1").arg(query.lastError().text()));
        return false;
    }
    
    // If this is set as default, unset others
    if (m_isDefault) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE bank_accounts SET is_default = 0 WHERE company_id = :company_id AND id != :id");
        updateQuery.bindValue(":company_id", m_companyId);
        updateQuery.bindValue(":id", m_id);
        updateQuery.exec();
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool BankAccount::remove()
{
    if (m_id.isEmpty()) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM bank_accounts WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove bank account:" << query.lastError().text();
        emit error(tr("Ошибка удаления счета: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void BankAccount::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setCompanyId(record.value("company_id").toString());
    setName(record.value("name").toString());
    setBankName(record.value("bank_name").toString());
    setAccountNumber(record.value("account_number").toString());
    setBik(record.value("bik").toString());
    setCurrency(record.value("currency").toString());
    setBalance(record.value("balance").toDouble());
    setIsDefault(record.value("is_default").toBool());
    setIsActive(record.value("is_active").toBool());
    setCreatedAt(record.value("created_at").toDateTime());
    setIsDirty(false);
}

QVariantMap BankAccount::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["companyId"] = m_companyId;
    map["name"] = m_name;
    map["bankName"] = m_bankName;
    map["accountNumber"] = m_accountNumber;
    map["maskedNumber"] = maskedNumber();
    map["bik"] = m_bik;
    map["currency"] = m_currency;
    map["balance"] = m_balance;
    map["formattedBalance"] = formattedBalance();
    map["isDefault"] = m_isDefault;
    map["isActive"] = m_isActive;
    map["displayName"] = displayName();
    return map;
}

void BankAccount::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("companyId")) setCompanyId(map["companyId"].toString());
    if (map.contains("name")) setName(map["name"].toString());
    if (map.contains("bankName")) setBankName(map["bankName"].toString());
    if (map.contains("accountNumber")) setAccountNumber(map["accountNumber"].toString());
    if (map.contains("bik")) setBik(map["bik"].toString());
    if (map.contains("currency")) setCurrency(map["currency"].toString());
    if (map.contains("balance")) setBalance(map["balance"].toDouble());
    if (map.contains("isDefault")) setIsDefault(map["isDefault"].toBool());
    if (map.contains("isActive")) setIsActive(map["isActive"].toBool());
}
