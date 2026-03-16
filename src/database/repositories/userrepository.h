#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QObject>
#include <QList>

class User;

class UserRepository : public QObject
{
    Q_OBJECT

public:
    static UserRepository* instance();
    
    // CRUD operations
    Q_INVOKABLE User* getById(const QString &id);
    Q_INVOKABLE User* getByEmail(const QString &email);
    Q_INVOKABLE QList<User*> getAll(const QString &companyId = QString());
    Q_INVOKABLE QList<User*> getActive(const QString &companyId = QString());
    
    // Authentication
    Q_INVOKABLE User* authenticate(const QString &email, const QString &password);
    Q_INVOKABLE bool changePassword(const QString &userId, const QString &oldPassword, const QString &newPassword);
    Q_INVOKABLE bool resetPassword(const QString &userId, const QString &newPassword);
    
    // Statistics
    Q_INVOKABLE int count(const QString &companyId = QString());
    Q_INVOKABLE int countByRole(int role, const QString &companyId = QString());

signals:
    void userSaved(const QString &id);
    void userRemoved(const QString &id);
    void error(const QString &message);

private:
    explicit UserRepository(QObject *parent = nullptr);
    static UserRepository *s_instance;
};

#endif // USERREPOSITORY_H
