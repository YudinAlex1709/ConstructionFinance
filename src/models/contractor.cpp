#include "contractor.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>

Contractor::Contractor(QObject *parent)
    : BaseModel(parent)
{
}

void Contractor::setType(ContractorType type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
        setIsDirty(true);
    }
}

void Contractor::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        setIsDirty(true);
    }
}

void Contractor::setFullName(const QString &name)
{
    if (m_fullName != name) {
        m_fullName = name;
        emit fullNameChanged();
        setIsDirty(true);
    }
}

void Contractor::setInn(const QString &inn)
{
    if (m_inn != inn) {
        m_inn = inn;
        emit innChanged();
        setIsDirty(true);
    }
}

void Contractor::setKpp(const QString &kpp)
{
    if (m_kpp != kpp) {
        m_kpp = kpp;
        emit kppChanged();
        setIsDirty(true);
    }
}

void Contractor::setOgrn(const QString &ogrn)
{
    if (m_ogrn != ogrn) {
        m_ogrn = ogrn;
        emit ogrnChanged();
        setIsDirty(true);
    }
}

void Contractor::setPhone(const QString &phone)
{
    if (m_phone != phone) {
        m_phone = phone;
        emit phoneChanged();
        setIsDirty(true);
    }
}

void Contractor::setEmail(const QString &email)
{
    if (m_email != email) {
        m_email = email;
        emit emailChanged();
        setIsDirty(true);
    }
}

void Contractor::setAddress(const QString &addr)
{
    if (m_address != addr) {
        m_address = addr;
        emit addressChanged();
        setIsDirty(true);
    }
}

void Contractor::setCity(const QString &city)
{
    if (m_city != city) {
        m_city = city;
        emit cityChanged();
        setIsDirty(true);
    }
}

void Contractor::setBankAccount(const QString &acc)
{
    if (m_bankAccount != acc) {
        m_bankAccount = acc;
        emit bankAccountChanged();
        setIsDirty(true);
    }
}

void Contractor::setBankName(const QString &name)
{
    if (m_bankName != name) {
        m_bankName = name;
        emit bankNameChanged();
        setIsDirty(true);
    }
}

void Contractor::setBik(const QString &bik)
{
    if (m_bik != bik) {
        m_bik = bik;
        emit bikChanged();
        setIsDirty(true);
    }
}

void Contractor::setNotes(const QString &notes)
{
    if (m_notes != notes) {
        m_notes = notes;
        emit notesChanged();
        setIsDirty(true);
    }
}

void Contractor::setIsActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        emit isActiveChanged();
        setIsDirty(true);
    }
}

QString Contractor::typeLabel() const
{
    switch (m_type) {
        case Client: return tr("Заказчик");
        case Supplier: return tr("Поставщик");
        case Subcontractor: return tr("Субподрядчик");
        case Employee: return tr("Сотрудник");
        case Other: return tr("Прочее");
        default: return tr("Неизвестно");
    }
}

QString Contractor::displayName() const
{
    if (!m_name.isEmpty()) return m_name;
    if (!m_fullName.isEmpty()) return m_fullName;
    return tr("Без названия");
}

bool Contractor::hasValidInn() const
{
    if (m_inn.isEmpty()) return false;
    // INN should be 10 or 12 digits
    QRegularExpression re("^\\d{10}$|^\\d{12}$");
    return re.match(m_inn).hasMatch();
}

QString Contractor::formattedPhone() const
{
    if (m_phone.isEmpty()) return QString();
    // Simple formatting: +7 (999) 123-45-67
    QString digits = m_phone;
    digits.remove(QRegularExpression("[^\\d]"));
    if (digits.startsWith("7") && digits.length() == 11) {
        return QString("+%1 (%2) %3-%4-%5")
            .arg(digits[0])
            .arg(digits.mid(1, 3))
            .arg(digits.mid(4, 3))
            .arg(digits.mid(7, 2))
            .arg(digits.mid(9, 2));
    }
    return m_phone;
}

bool Contractor::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO contractors 
        (id, company_id, type, name, full_name, inn, kpp, ogrn, phone, email,
         address, city, bank_account, bank_name, bik, notes, is_active, created_at)
        VALUES 
        (:id, :company_id, :type, :name, :full_name, :inn, :kpp, :ogrn, :phone, :email,
         :address, :city, :bank_account, :bank_name, :bik, :notes, :is_active, :created_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":company_id", "default"); // TODO: get from current user
    query.bindValue(":type", static_cast<int>(m_type));
    query.bindValue(":name", m_name);
    query.bindValue(":full_name", m_fullName);
    query.bindValue(":inn", m_inn);
    query.bindValue(":kpp", m_kpp);
    query.bindValue(":ogrn", m_ogrn);
    query.bindValue(":phone", m_phone);
    query.bindValue(":email", m_email);
    query.bindValue(":address", m_address);
    query.bindValue(":city", m_city);
    query.bindValue(":bank_account", m_bankAccount);
    query.bindValue(":bank_name", m_bankName);
    query.bindValue(":bik", m_bik);
    query.bindValue(":notes", m_notes);
    query.bindValue(":is_active", m_isActive ? 1 : 0);
    query.bindValue(":created_at", m_createdAt);
    
    if (!query.exec()) {
        qWarning() << "Failed to save contractor:" << query.lastError().text();
        emit error(tr("Ошибка сохранения контрагента: %1").arg(query.lastError().text()));
        return false;
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool Contractor::remove()
{
    if (m_id.isEmpty()) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM contractors WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove contractor:" << query.lastError().text();
        emit error(tr("Ошибка удаления контрагента: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void Contractor::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setType(static_cast<ContractorType>(record.value("type").toInt()));
    setName(record.value("name").toString());
    setFullName(record.value("full_name").toString());
    setInn(record.value("inn").toString());
    setKpp(record.value("kpp").toString());
    setOgrn(record.value("ogrn").toString());
    setPhone(record.value("phone").toString());
    setEmail(record.value("email").toString());
    setAddress(record.value("address").toString());
    setCity(record.value("city").toString());
    setBankAccount(record.value("bank_account").toString());
    setBankName(record.value("bank_name").toString());
    setBik(record.value("bik").toString());
    setNotes(record.value("notes").toString());
    setIsActive(record.value("is_active").toBool());
    setCreatedAt(record.value("created_at").toDateTime());
    setIsDirty(false);
}

QVariantMap Contractor::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["type"] = static_cast<int>(m_type);
    map["typeLabel"] = typeLabel();
    map["name"] = m_name;
    map["fullName"] = m_fullName;
    map["displayName"] = displayName();
    map["inn"] = m_inn;
    map["kpp"] = m_kpp;
    map["ogrn"] = m_ogrn;
    map["phone"] = m_phone;
    map["formattedPhone"] = formattedPhone();
    map["email"] = m_email;
    map["address"] = m_address;
    map["city"] = m_city;
    map["bankAccount"] = m_bankAccount;
    map["bankName"] = m_bankName;
    map["bik"] = m_bik;
    map["notes"] = m_notes;
    map["isActive"] = m_isActive;
    return map;
}

void Contractor::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("type")) setType(static_cast<ContractorType>(map["type"].toInt()));
    if (map.contains("name")) setName(map["name"].toString());
    if (map.contains("fullName")) setFullName(map["fullName"].toString());
    if (map.contains("inn")) setInn(map["inn"].toString());
    if (map.contains("kpp")) setKpp(map["kpp"].toString());
    if (map.contains("ogrn")) setOgrn(map["ogrn"].toString());
    if (map.contains("phone")) setPhone(map["phone"].toString());
    if (map.contains("email")) setEmail(map["email"].toString());
    if (map.contains("address")) setAddress(map["address"].toString());
    if (map.contains("city")) setCity(map["city"].toString());
    if (map.contains("bankAccount")) setBankAccount(map["bankAccount"].toString());
    if (map.contains("bankName")) setBankName(map["bankName"].toString());
    if (map.contains("bik")) setBik(map["bik"].toString());
    if (map.contains("notes")) setNotes(map["notes"].toString());
    if (map.contains("isActive")) setIsActive(map["isActive"].toBool());
}
