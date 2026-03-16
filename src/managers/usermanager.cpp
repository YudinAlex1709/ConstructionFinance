#include "usermanager.h"
#include "../models/user.h"
#include "../database/repositories/userrepository.h"
#include <QDebug>

UserManager* UserManager::s_instance = nullptr;

UserManager* UserManager::instance()
{
    if (!s_instance) {
        s_instance = new UserManager();
    }
    return s_instance;
}

UserManager::UserManager(QObject *parent)
    : QObject(parent)
{
    // Create default admin user if no users exist
    if (UserRepository::instance()->count() == 0) {
        User *admin = new User();
        admin->setEmail("admin@construction.finance");
        admin->setPasswordHash(User::hashPassword("admin"));
        admin->setFirstName("Admin");
        admin->setLastName("User");
        admin->setRole(User::Admin);
        admin->setCompanyId("default");
        admin->save();
        delete admin;
        qDebug() << "Default admin user created";
    }
}

bool UserManager::login(const QString &email, const QString &password)
{
    User *user = UserRepository::instance()->authenticate(email, password);
    
    if (!user) {
        emit loginFailed(tr("Неверный email или пароль"));
        return false;
    }
    
    setCurrentUser(user);
    emit loginSuccess();
    return true;
}

void UserManager::logout()
{
    setCurrentUser(nullptr);
    emit logoutSuccess();
}

bool UserManager::registerUser(const QVariantMap &data)
{
    // Check if email already exists
    QString email = data["email"].toString();
    if (UserRepository::instance()->getByEmail(email)) {
        emit error(tr("Пользователь с таким email уже существует"));
        return false;
    }
    
    User *user = new User();
    user->fromMap(data);
    user->setCompanyId("default");
    
    QString password = data["password"].toString();
    if (!password.isEmpty()) {
        user->setPasswordHash(User::hashPassword(password));
    }
    
    if (!user->save()) {
        delete user;
        emit error(tr("Не удалось создать пользователя"));
        return false;
    }
    
    delete user;
    loadUsers();
    return true;
}

bool UserManager::updateUser(const QString &id, const QVariantMap &data)
{
    User *user = UserRepository::instance()->getById(id);
    if (!user) {
        emit error(tr("Пользователь не найден"));
        return false;
    }
    
    user->fromMap(data);
    
    QString password = data["password"].toString();
    if (!password.isEmpty()) {
        user->setPasswordHash(User::hashPassword(password));
    }
    
    if (!user->save()) {
        delete user;
        emit error(tr("Не удалось обновить пользователя"));
        return false;
    }
    
    delete user;
    loadUsers();
    return true;
}

bool UserManager::deleteUser(const QString &id)
{
    // Don't allow deleting yourself
    if (m_currentUser && m_currentUser->id() == id) {
        emit error(tr("Нельзя удалить текущего пользователя"));
        return false;
    }
    
    User *user = UserRepository::instance()->getById(id);
    if (!user) {
        emit error(tr("Пользователь не найден"));
        return false;
    }
    
    if (!user->remove()) {
        delete user;
        emit error(tr("Не удалось удалить пользователя"));
        return false;
    }
    
    delete user;
    loadUsers();
    return true;
}

bool UserManager::changePassword(const QString &oldPassword, const QString &newPassword)
{
    if (!m_currentUser) {
        emit error(tr("Пользователь не авторизован"));
        return false;
    }
    
    if (!UserRepository::instance()->changePassword(m_currentUser->id(), oldPassword, newPassword)) {
        emit error(tr("Не удалось изменить пароль"));
        return false;
    }
    
    return true;
}

void UserManager::loadUsers()
{
    QList<User*> users = UserRepository::instance()->getAll("default");
    
    m_allUsers.clear();
    for (User *user : users) {
        m_allUsers.append(user->toMap());
        delete user;
    }
    
    emit usersChanged();
}

void UserManager::setCurrentUser(User *user)
{
    if (m_currentUser) {
        delete m_currentUser;
    }
    
    m_currentUser = user;
    emit currentUserChanged();
    emit loginStateChanged();
}
