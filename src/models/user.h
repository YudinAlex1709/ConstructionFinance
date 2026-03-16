#ifndef USER_H
#define USER_H

#include "basemodel.h"

class User : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString fullName READ fullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString initials READ initials NOTIFY initialsChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(UserRole role READ role WRITE setRole NOTIFY roleChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
    Q_PROPERTY(QString companyId READ companyId WRITE setCompanyId NOTIFY companyIdChanged)
    Q_PROPERTY(QString roleLabel READ roleLabel NOTIFY roleChanged)
    Q_PROPERTY(bool isAdmin READ isAdmin NOTIFY roleChanged)

public:
    enum UserRole {
        Admin = 0,          // Администратор
        Director = 1,       // Директор
        FinanceManager = 2, // Финансовый менеджер
        ProjectManager = 3, // Проектный менеджер
        Manager = 4,        // Менеджер
        Accountant = 5,     // Бухгалтер
        Viewer = 6          // Только просмотр
    };
    Q_ENUM(UserRole)

    explicit User(QObject *parent = nullptr);
    
    // Getters
    QString email() const { return m_email; }
    QString firstName() const { return m_firstName; }
    QString lastName() const { return m_lastName; }
    QString fullName() const;
    QString initials() const;
    QString phone() const { return m_phone; }
    UserRole role() const { return m_role; }
    bool isActive() const { return m_isActive; }
    QString companyId() const { return m_companyId; }
    
    // Setters
    void setEmail(const QString &email);
    void setFirstName(const QString &name);
    void setLastName(const QString &name);
    void setPhone(const QString &phone);
    void setRole(UserRole role);
    void setIsActive(bool active);
    void setCompanyId(const QString &id);
    
    // Helper methods
    QString roleLabel() const;
    bool isAdmin() const { return m_role == Admin; }
    Q_INVOKABLE bool hasPermission(const QString &permission) const;
    
    // Password handling (not exposed to QML)
    void setPasswordHash(const QString &hash);
    QString passwordHash() const { return m_passwordHash; }
    static QString hashPassword(const QString &password);
    bool verifyPassword(const QString &password) const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void emailChanged();
    void firstNameChanged();
    void lastNameChanged();
    void fullNameChanged();
    void initialsChanged();
    void phoneChanged();
    void roleChanged();
    void isActiveChanged();
    void companyIdChanged();

private:
    QString m_email;
    QString m_firstName;
    QString m_lastName;
    QString m_phone;
    UserRole m_role = Manager;
    bool m_isActive = true;
    QString m_companyId;
    QString m_passwordHash;
};

#endif // USER_H
