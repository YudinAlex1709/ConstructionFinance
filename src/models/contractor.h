#ifndef CONTRACTOR_H
#define CONTRACTOR_H

#include "basemodel.h"

class Contractor : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(ContractorType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString fullName READ fullName WRITE setFullName NOTIFY fullNameChanged)
    Q_PROPERTY(QString inn READ inn WRITE setInn NOTIFY innChanged)
    Q_PROPERTY(QString kpp READ kpp WRITE setKpp NOTIFY kppChanged)
    Q_PROPERTY(QString ogrn READ ogrn WRITE setOgrn NOTIFY ogrnChanged)
    Q_PROPERTY(QString phone READ phone WRITE setPhone NOTIFY phoneChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(QString address READ address WRITE setAddress NOTIFY addressChanged)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString bankAccount READ bankAccount WRITE setBankAccount NOTIFY bankAccountChanged)
    Q_PROPERTY(QString bankName READ bankName WRITE setBankName NOTIFY bankNameChanged)
    Q_PROPERTY(QString bik READ bik WRITE setBik NOTIFY bikChanged)
    Q_PROPERTY(QString notes READ notes WRITE setNotes NOTIFY notesChanged)
    Q_PROPERTY(bool isActive READ isActive WRITE setIsActive NOTIFY isActiveChanged)
    Q_PROPERTY(QString typeLabel READ typeLabel NOTIFY typeChanged)
    Q_PROPERTY(QString displayName READ displayName NOTIFY nameChanged)

public:
    enum ContractorType {
        Client = 0,         // Заказчик
        Supplier = 1,       // Поставщик
        Subcontractor = 2,  // Субподрядчик
        Employee = 3,       // Сотрудник
        Other = 4           // Прочее
    };
    Q_ENUM(ContractorType)

    explicit Contractor(QObject *parent = nullptr);
    
    // Getters
    ContractorType type() const { return m_type; }
    QString name() const { return m_name; }
    QString fullName() const { return m_fullName; }
    QString inn() const { return m_inn; }
    QString kpp() const { return m_kpp; }
    QString ogrn() const { return m_ogrn; }
    QString phone() const { return m_phone; }
    QString email() const { return m_email; }
    QString address() const { return m_address; }
    QString city() const { return m_city; }
    QString bankAccount() const { return m_bankAccount; }
    QString bankName() const { return m_bankName; }
    QString bik() const { return m_bik; }
    QString notes() const { return m_notes; }
    bool isActive() const { return m_isActive; }
    
    // Setters
    void setType(ContractorType type);
    void setName(const QString &name);
    void setFullName(const QString &name);
    void setInn(const QString &inn);
    void setKpp(const QString &kpp);
    void setOgrn(const QString &ogrn);
    void setPhone(const QString &phone);
    void setEmail(const QString &email);
    void setAddress(const QString &addr);
    void setCity(const QString &city);
    void setBankAccount(const QString &acc);
    void setBankName(const QString &name);
    void setBik(const QString &bik);
    void setNotes(const QString &notes);
    void setIsActive(bool active);
    
    // Helper methods
    QString typeLabel() const;
    QString displayName() const;
    Q_INVOKABLE bool hasValidInn() const;
    Q_INVOKABLE QString formattedPhone() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void typeChanged();
    void nameChanged();
    void fullNameChanged();
    void innChanged();
    void kppChanged();
    void ogrnChanged();
    void phoneChanged();
    void emailChanged();
    void addressChanged();
    void cityChanged();
    void bankAccountChanged();
    void bankNameChanged();
    void bikChanged();
    void notesChanged();
    void isActiveChanged();

private:
    ContractorType m_type = Client;
    QString m_name;
    QString m_fullName;
    QString m_inn;
    QString m_kpp;
    QString m_ogrn;
    QString m_phone;
    QString m_email;
    QString m_address;
    QString m_city;
    QString m_bankAccount;
    QString m_bankName;
    QString m_bik;
    QString m_notes;
    bool m_isActive = true;
};

#endif // CONTRACTOR_H
