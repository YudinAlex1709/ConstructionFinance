#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QObject>
#include <QDateTime>
#include <QUuid>
#include <QSqlRecord>

class BaseModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(QDateTime updatedAt READ updatedAt WRITE setUpdatedAt NOTIFY updatedAtChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isDirty READ isDirty WRITE setIsDirty NOTIFY isDirtyChanged)

public:
    explicit BaseModel(QObject *parent = nullptr);
    virtual ~BaseModel() = default;
    
    QString id() const { return m_id; }
    void setId(const QString &id) { 
        if (m_id != id) {
            m_id = id;
            emit idChanged();
            setIsDirty(true);
        }
    }
    
    QDateTime createdAt() const { return m_createdAt; }
    void setCreatedAt(const QDateTime &dt) {
        if (m_createdAt != dt) {
            m_createdAt = dt;
            emit createdAtChanged();
        }
    }
    
    QDateTime updatedAt() const { return m_updatedAt; }
    void setUpdatedAt(const QDateTime &dt) {
        if (m_updatedAt != dt) {
            m_updatedAt = dt;
            emit updatedAtChanged();
            setIsDirty(true);
        }
    }
    
    bool isValid() const { return !m_id.isEmpty(); }
    
    bool isDirty() const { return m_isDirty; }
    void setIsDirty(bool dirty) {
        if (m_isDirty != dirty) {
            m_isDirty = dirty;
            emit isDirtyChanged();
        }
    }
    
    virtual void generateId() {
        setId(QUuid::createUuid().toString(QUuid::WithoutBraces));
    }
    
    Q_INVOKABLE virtual bool save() = 0;
    Q_INVOKABLE virtual bool remove() = 0;
    Q_INVOKABLE virtual void loadFromRecord(const QSqlRecord &record) = 0;
    Q_INVOKABLE virtual QVariantMap toMap() const = 0;
    Q_INVOKABLE virtual void fromMap(const QVariantMap &map) = 0;

signals:
    void idChanged();
    void createdAtChanged();
    void updatedAtChanged();
    void isValidChanged();
    void isDirtyChanged();
    void saved();
    void removed();
    void error(const QString &message);

protected:
    QString m_id;
    QDateTime m_createdAt;
    QDateTime m_updatedAt;
    bool m_isDirty = false;
};

#endif // BASEMODEL_H
