#include "userrepository.h"
#include "../../models/user.h"
#include "../database.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDebug>

UserRepository* UserRepository::s_instance = nullptr;

UserRepository* UserRepository::instance()
{
    if (!s_instance) {
        s_instance = new UserRepository();
    }
    return s_instance;
}

UserRepository::UserRepository(QObject *parent)
    : QObject(parent)
{
}

User* UserRepository::getById(const QString &id)
{
    QSqlQuery query = Database::instance()->execute(
        "SELECT * FROM users WHERE id = :id",
        {{":id", id}}
    );
    
    if (query.next()) {
        User *user = new User();
        user->loadFromRecord(query.record());
        return user;
    }
    
    return nullptr;
}

User* UserRepository::getByEmail(const QString &email)
{
    QSqlQuery query = Database::instance()->execute(
        "SELECT * FROM users WHERE email = :email",
        {{":email", email.toLower()}}
    );
    
    if (query.next()) {
        User *user = new User();
        user->loadFromRecord(query.record());
        return user;
    }
    
    return nullptr;
}

QList<User*> UserRepository::getAll(const QString &companyId)
{
    QList<User*> users;
    
    QString sql = "SELECT * FROM users";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " WHERE company_id = :company_id";
        params[":company_id"] = companyId;
    }
    sql += " ORDER BY created_at DESC";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        User *user = new User();
        user->loadFromRecord(query.record());
        users.append(user);
    }
    
    return users;
}

QList<User*> UserRepository::getActive(const QString &companyId)
{
    QList<User*> users;
    
    QString sql = "SELECT * FROM users WHERE is_active = 1";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    sql += " ORDER BY last_name, first_name";
    
    QSqlQuery query = Database::instance()->execute(sql, params);
    
    while (query.next()) {
        User *user = new User();
        user->loadFromRecord(query.record());
        users.append(user);
    }
    
    return users;
}

User* UserRepository::authenticate(const QString &email, const QString &password)
{
    User *user = getByEmail(email);
    if (!user) {
        return nullptr;
    }
    
    if (!user->verifyPassword(password)) {
        delete user;
        return nullptr;
    }
    
    if (!user->isActive()) {
        delete user;
        return nullptr;
    }
    
    return user;
}

bool UserRepository::changePassword(const QString &userId, const QString &oldPassword, const QString &newPassword)
{
    User *user = getById(userId);
    if (!user) {
        emit error(tr("Пользователь не найден"));
        return false;
    }
    
    if (!user->verifyPassword(oldPassword)) {
        emit error(tr("Неверный текущий пароль"));
        delete user;
        return false;
    }
    
    user->setPasswordHash(User::hashPassword(newPassword));
    bool result = user->save();
    delete user;
    
    return result;
}

bool UserRepository::resetPassword(const QString &userId, const QString &newPassword)
{
    User *user = getById(userId);
    if (!user) {
        emit error(tr("Пользователь не найден"));
        return false;
    }
    
    user->setPasswordHash(User::hashPassword(newPassword));
    bool result = user->save();
    delete user;
    
    return result;
}

int UserRepository::count(const QString &companyId)
{
    QString sql = "SELECT COUNT(*) FROM users";
    QVariantMap params;
    
    if (!companyId.isEmpty()) {
        sql += " WHERE company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryInt(sql, params);
}

int UserRepository::countByRole(int role, const QString &companyId)
{
    QString sql = "SELECT COUNT(*) FROM users WHERE role = :role";
    QVariantMap params;
    params[":role"] = role;
    
    if (!companyId.isEmpty()) {
        sql += " AND company_id = :company_id";
        params[":company_id"] = companyId;
    }
    
    return Database::instance()->queryInt(sql, params);
}
