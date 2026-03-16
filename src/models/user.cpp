#include "user.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QCryptographicHash>
#include <QRegularExpression>

User::User(QObject *parent)
    : BaseModel(parent)
{
}

QString User::fullName() const
{
    if (!m_firstName.isEmpty() && !m_lastName.isEmpty()) {
        return m_firstName + " " + m_lastName;
    }
    return m_firstName.isEmpty() ? m_lastName : m_firstName;
}

QString User::initials() const
{
    QString result;
    if (!m_firstName.isEmpty()) {
        result += m_firstName[0].toUpper();
    }
    if (!m_lastName.isEmpty()) {
        result += m_lastName[0].toUpper();
    }
    return result;
}

void User::setEmail(const QString &email)
{
    QString lowerEmail = email.toLower();
    if (m_email != lowerEmail) {
        m_email = lowerEmail;
        emit emailChanged();
        setIsDirty(true);
    }
}

void User::setFirstName(const QString &name)
{
    if (m_firstName != name) {
        m_firstName = name;
        emit firstNameChanged();
        emit fullNameChanged();
        emit initialsChanged();
        setIsDirty(true);
    }
}

void User::setLastName(const QString &name)
{
    if (m_lastName != name) {
        m_lastName = name;
        emit lastNameChanged();
        emit fullNameChanged();
        emit initialsChanged();
        setIsDirty(true);
    }
}

void User::setPhone(const QString &phone)
{
    if (m_phone != phone) {
        m_phone = phone;
        emit phoneChanged();
        setIsDirty(true);
    }
}

void User::setRole(UserRole role)
{
    if (m_role != role) {
        m_role = role;
        emit roleChanged();
        setIsDirty(true);
    }
}

void User::setIsActive(bool active)
{
    if (m_isActive != active) {
        m_isActive = active;
        emit isActiveChanged();
        setIsDirty(true);
    }
}

void User::setCompanyId(const QString &id)
{
    if (m_companyId != id) {
        m_companyId = id;
        emit companyIdChanged();
        setIsDirty(true);
    }
}

QString User::roleLabel() const
{
    switch (m_role) {
        case Admin: return tr("Администратор");
        case Director: return tr("Директор");
        case FinanceManager: return tr("Финансовый менеджер");
        case ProjectManager: return tr("Проектный менеджер");
        case Manager: return tr("Менеджер");
        case Accountant: return tr("Бухгалтер");
        case Viewer: return tr("Наблюдатель");
        default: return tr("Неизвестно");
    }
}

bool User::hasPermission(const QString &permission) const
{
    // Simple permission system based on roles
    switch (m_role) {
        case Admin:
            return true;
        case Director:
            return permission != "delete_company";
        case FinanceManager:
            return permission.startsWith("view") || 
                   permission.startsWith("finance") ||
                   permission == "view_reports";
        case ProjectManager:
            return permission.startsWith("view") || 
                   permission.startsWith("project") ||
                   permission == "view_finance";
        case Manager:
            return permission.startsWith("view") || 
                   permission == "edit_project" ||
                   permission == "create_transaction";
        case Accountant:
            return permission.startsWith("view") || 
                   permission.startsWith("finance") ||
                   permission == "edit_transaction";
        case Viewer:
            return permission.startsWith("view");
        default:
            return false;
    }
}

void User::setPasswordHash(const QString &hash)
{
    m_passwordHash = hash;
    setIsDirty(true);
}

QString User::hashPassword(const QString &password)
{
    // Simple SHA-256 hash (in production, use bcrypt or Argon2)
    return QString(QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex());
}

bool User::verifyPassword(const QString &password) const
{
    return m_passwordHash == hashPassword(password);
}

bool User::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    // Validate email
    QRegularExpression emailRe("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
    if (!emailRe.match(m_email).hasMatch()) {
        emit error(tr("Некорректный email адрес"));
        return false;
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO users 
        (id, email, password_hash, first_name, last_name, phone, role, is_active, company_id, created_at, updated_at)
        VALUES 
        (:id, :email, :password_hash, :first_name, :last_name, :phone, :role, :is_active, :company_id, :created_at, :updated_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":email", m_email);
    query.bindValue(":password_hash", m_passwordHash);
    query.bindValue(":first_name", m_firstName);
    query.bindValue(":last_name", m_lastName);
    query.bindValue(":phone", m_phone);
    query.bindValue(":role", static_cast<int>(m_role));
    query.bindValue(":is_active", m_isActive ? 1 : 0);
    query.bindValue(":company_id", m_companyId);
    query.bindValue(":created_at", m_createdAt);
    query.bindValue(":updated_at", QDateTime::currentDateTime());
    
    if (!query.exec()) {
        qWarning() << "Failed to save user:" << query.lastError().text();
        emit error(tr("Ошибка сохранения пользователя: %1").arg(query.lastError().text()));
        return false;
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool User::remove()
{
    if (m_id.isEmpty()) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM users WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove user:" << query.lastError().text();
        emit error(tr("Ошибка удаления пользователя: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void User::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setEmail(record.value("email").toString());
    m_passwordHash = record.value("password_hash").toString();
    setFirstName(record.value("first_name").toString());
    setLastName(record.value("last_name").toString());
    setPhone(record.value("phone").toString());
    setRole(static_cast<UserRole>(record.value("role").toInt()));
    setIsActive(record.value("is_active").toBool());
    setCompanyId(record.value("company_id").toString());
    setCreatedAt(record.value("created_at").toDateTime());
    setUpdatedAt(record.value("updated_at").toDateTime());
    setIsDirty(false);
}

QVariantMap User::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["email"] = m_email;
    map["firstName"] = m_firstName;
    map["lastName"] = m_lastName;
    map["fullName"] = fullName();
    map["initials"] = initials();
    map["phone"] = m_phone;
    map["role"] = static_cast<int>(m_role);
    map["roleLabel"] = roleLabel();
    map["isActive"] = m_isActive;
    map["companyId"] = m_companyId;
    map["isAdmin"] = isAdmin();
    return map;
}

void User::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("email")) setEmail(map["email"].toString());
    if (map.contains("firstName")) setFirstName(map["firstName"].toString());
    if (map.contains("lastName")) setLastName(map["lastName"].toString());
    if (map.contains("phone")) setPhone(map["phone"].toString());
    if (map.contains("role")) setRole(static_cast<UserRole>(map["role"].toInt()));
    if (map.contains("isActive")) setIsActive(map["isActive"].toBool());
    if (map.contains("companyId")) setCompanyId(map["companyId"].toString());
}
