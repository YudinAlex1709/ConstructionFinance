#ifndef CATEGORY_H
#define CATEGORY_H

#include "basemodel.h"

class Category : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QString companyId READ companyId WRITE setCompanyId NOTIFY companyIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(CategoryType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QString icon READ icon WRITE setIcon NOTIFY iconChanged)
    Q_PROPERTY(QString parentId READ parentId WRITE setParentId NOTIFY parentIdChanged)
    Q_PROPERTY(bool isSystem READ isSystem WRITE setIsSystem NOTIFY isSystemChanged)
    Q_PROPERTY(QString typeLabel READ typeLabel NOTIFY typeChanged)

public:
    enum CategoryType {
        Income = 0,
        Expense = 1
    };
    Q_ENUM(CategoryType)

    explicit Category(QObject *parent = nullptr);
    
    // Getters
    QString companyId() const { return m_companyId; }
    QString name() const { return m_name; }
    CategoryType type() const { return m_type; }
    QString color() const { return m_color; }
    QString icon() const { return m_icon; }
    QString parentId() const { return m_parentId; }
    bool isSystem() const { return m_isSystem; }
    
    // Setters
    void setCompanyId(const QString &id);
    void setName(const QString &name);
    void setType(CategoryType type);
    void setColor(const QString &color);
    void setIcon(const QString &icon);
    void setParentId(const QString &id);
    void setIsSystem(bool isSystem);
    
    // Helper methods
    QString typeLabel() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void companyIdChanged();
    void nameChanged();
    void typeChanged();
    void colorChanged();
    void iconChanged();
    void parentIdChanged();
    void isSystemChanged();

private:
    QString m_companyId;
    QString m_name;
    CategoryType m_type = Expense;
    QString m_color = "#3B82F6";
    QString m_icon;
    QString m_parentId;
    bool m_isSystem = false;
};

#endif // CATEGORY_H
