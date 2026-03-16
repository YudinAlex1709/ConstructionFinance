#ifndef USERMANAGER_H
#define USERMANAGER_H

#include <QObject>
#include <QVariantMap>

class User;

class UserManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLoggedIn READ isLoggedIn NOTIFY loginStateChanged)
    Q_PROPERTY(User* currentUser READ currentUser NOTIFY currentUserChanged)
    Q_PROPERTY(QVariantList allUsers READ allUsers NOTIFY usersChanged)

public:
    static UserManager* instance();
    
    bool isLoggedIn() const { return m_currentUser != nullptr; }
    User* currentUser() const { return m_currentUser; }
    QVariantList allUsers() const { return m_allUsers; }
    
    Q_INVOKABLE bool login(const QString &email, const QString &password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE bool registerUser(const QVariantMap &data);
    Q_INVOKABLE bool updateUser(const QString &id, const QVariantMap &data);
    Q_INVOKABLE bool deleteUser(const QString &id);
    Q_INVOKABLE bool changePassword(const QString &oldPassword, const QString &newPassword);
    
    Q_INVOKABLE void loadUsers();

signals:
    void loginStateChanged();
    void currentUserChanged();
    void usersChanged();
    void loginSuccess();
    void loginFailed(const QString &reason);
    void logoutSuccess();
    void error(const QString &message);

private:
    explicit UserManager(QObject *parent = nullptr);
    void setCurrentUser(User *user);
    
    User *m_currentUser = nullptr;
    QVariantList m_allUsers;
    
    static UserManager *s_instance;
};

#endif // USERMANAGER_H
