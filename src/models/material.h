#ifndef MATERIAL_H
#define MATERIAL_H

#include "basemodel.h"
#include <QDate>

class Material : public BaseModel
{
    Q_OBJECT
    Q_PROPERTY(QString projectId READ projectId WRITE setProjectId NOTIFY projectIdChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString unit READ unit WRITE setUnit NOTIFY unitChanged)
    Q_PROPERTY(double quantity READ quantity WRITE setQuantity NOTIFY quantityChanged)
    Q_PROPERTY(double price READ price WRITE setPrice NOTIFY priceChanged)
    Q_PROPERTY(double total READ total NOTIFY totalChanged)
    Q_PROPERTY(QString supplierId READ supplierId WRITE setSupplierId NOTIFY supplierIdChanged)
    Q_PROPERTY(QString supplierName READ supplierName WRITE setSupplierName NOTIFY supplierNameChanged)
    Q_PROPERTY(QDate materialDate READ materialDate WRITE setMaterialDate NOTIFY materialDateChanged)
    Q_PROPERTY(QString notes READ notes WRITE setNotes NOTIFY notesChanged)

public:
    explicit Material(QObject *parent = nullptr);
    
    // Getters
    QString projectId() const { return m_projectId; }
    QString name() const { return m_name; }
    QString unit() const { return m_unit; }
    double quantity() const { return m_quantity; }
    double price() const { return m_price; }
    double total() const { return m_quantity * m_price; }
    QString supplierId() const { return m_supplierId; }
    QString supplierName() const { return m_supplierName; }
    QDate materialDate() const { return m_materialDate; }
    QString notes() const { return m_notes; }
    
    // Setters
    void setProjectId(const QString &id);
    void setName(const QString &name);
    void setUnit(const QString &unit);
    void setQuantity(double qty);
    void setPrice(double price);
    void setSupplierId(const QString &id);
    void setSupplierName(const QString &name);
    void setMaterialDate(const QDate &date);
    void setNotes(const QString &notes);
    
    // Helper methods
    Q_INVOKABLE QString formattedTotal() const;
    Q_INVOKABLE QString formattedQuantity() const;
    
    // Database operations
    Q_INVOKABLE bool save() override;
    Q_INVOKABLE bool remove() override;
    Q_INVOKABLE void loadFromRecord(const QSqlRecord &record) override;
    Q_INVOKABLE QVariantMap toMap() const override;
    Q_INVOKABLE void fromMap(const QVariantMap &map) override;

signals:
    void projectIdChanged();
    void nameChanged();
    void unitChanged();
    void quantityChanged();
    void priceChanged();
    void totalChanged();
    void supplierIdChanged();
    void supplierNameChanged();
    void materialDateChanged();
    void notesChanged();

private:
    QString m_projectId;
    QString m_name;
    QString m_unit = "шт";
    double m_quantity = 0.0;
    double m_price = 0.0;
    QString m_supplierId;
    QString m_supplierName;
    QDate m_materialDate;
    QString m_notes;
};

#endif // MATERIAL_H
