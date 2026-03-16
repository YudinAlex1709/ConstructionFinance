#include "material.h"
#include "../database/database.h"
#include "../utils/currencyutils.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Material::Material(QObject *parent)
    : BaseModel(parent)
{
}

void Material::setProjectId(const QString &id)
{
    if (m_projectId != id) {
        m_projectId = id;
        emit projectIdChanged();
        setIsDirty(true);
    }
}

void Material::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        setIsDirty(true);
    }
}

void Material::setUnit(const QString &unit)
{
    if (m_unit != unit) {
        m_unit = unit;
        emit unitChanged();
        setIsDirty(true);
    }
}

void Material::setQuantity(double qty)
{
    qty = qMax(0.0, qty);
    if (!qFuzzyCompare(m_quantity, qty)) {
        m_quantity = qty;
        emit quantityChanged();
        emit totalChanged();
        setIsDirty(true);
    }
}

void Material::setPrice(double price)
{
    price = qMax(0.0, price);
    if (!qFuzzyCompare(m_price, price)) {
        m_price = price;
        emit priceChanged();
        emit totalChanged();
        setIsDirty(true);
    }
}

void Material::setSupplierId(const QString &id)
{
    if (m_supplierId != id) {
        m_supplierId = id;
        emit supplierIdChanged();
        setIsDirty(true);
    }
}

void Material::setSupplierName(const QString &name)
{
    if (m_supplierName != name) {
        m_supplierName = name;
        emit supplierNameChanged();
    }
}

void Material::setMaterialDate(const QDate &date)
{
    if (m_materialDate != date) {
        m_materialDate = date;
        emit materialDateChanged();
        setIsDirty(true);
    }
}

void Material::setNotes(const QString &notes)
{
    if (m_notes != notes) {
        m_notes = notes;
        emit notesChanged();
        setIsDirty(true);
    }
}

QString Material::formattedTotal() const
{
    return CurrencyUtils::format(total());
}

QString Material::formattedQuantity() const
{
    return QString::number(m_quantity, 'f', 2) + " " + m_unit;
}

bool Material::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    if (!m_materialDate.isValid()) {
        m_materialDate = QDate::currentDate();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO materials 
        (id, project_id, name, unit, quantity, price, total, supplier_id, material_date, notes, created_at)
        VALUES 
        (:id, :project_id, :name, :unit, :quantity, :price, :total, :supplier_id, :material_date, :notes, :created_at)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":project_id", m_projectId);
    query.bindValue(":name", m_name);
    query.bindValue(":unit", m_unit);
    query.bindValue(":quantity", m_quantity);
    query.bindValue(":price", m_price);
    query.bindValue(":total", total());
    query.bindValue(":supplier_id", m_supplierId);
    query.bindValue(":material_date", m_materialDate);
    query.bindValue(":notes", m_notes);
    query.bindValue(":created_at", m_createdAt);
    
    if (!query.exec()) {
        qWarning() << "Failed to save material:" << query.lastError().text();
        emit error(tr("Ошибка сохранения материала: %1").arg(query.lastError().text()));
        return false;
    }
    
    // Update project budget spent
    if (!m_projectId.isEmpty()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE projects SET budget_spent = budget_spent + :amount WHERE id = :project_id");
        updateQuery.bindValue(":amount", total());
        updateQuery.bindValue(":project_id", m_projectId);
        updateQuery.exec();
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool Material::remove()
{
    if (m_id.isEmpty()) return false;
    
    // Subtract from project budget spent
    if (!m_projectId.isEmpty()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE projects SET budget_spent = budget_spent - :amount WHERE id = :project_id");
        updateQuery.bindValue(":amount", total());
        updateQuery.bindValue(":project_id", m_projectId);
        updateQuery.exec();
    }
    
    QSqlQuery query;
    query.prepare("DELETE FROM materials WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove material:" << query.lastError().text();
        emit error(tr("Ошибка удаления материала: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void Material::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setProjectId(record.value("project_id").toString());
    setName(record.value("name").toString());
    setUnit(record.value("unit").toString());
    setQuantity(record.value("quantity").toDouble());
    setPrice(record.value("price").toDouble());
    setSupplierId(record.value("supplier_id").toString());
    setMaterialDate(record.value("material_date").toDate());
    setNotes(record.value("notes").toString());
    setCreatedAt(record.value("created_at").toDateTime());
    setIsDirty(false);
}

QVariantMap Material::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["projectId"] = m_projectId;
    map["name"] = m_name;
    map["unit"] = m_unit;
    map["quantity"] = m_quantity;
    map["price"] = m_price;
    map["total"] = total();
    map["formattedTotal"] = formattedTotal();
    map["formattedQuantity"] = formattedQuantity();
    map["supplierId"] = m_supplierId;
    map["supplierName"] = m_supplierName;
    map["materialDate"] = m_materialDate;
    map["notes"] = m_notes;
    return map;
}

void Material::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("projectId")) setProjectId(map["projectId"].toString());
    if (map.contains("name")) setName(map["name"].toString());
    if (map.contains("unit")) setUnit(map["unit"].toString());
    if (map.contains("quantity")) setQuantity(map["quantity"].toDouble());
    if (map.contains("price")) setPrice(map["price"].toDouble());
    if (map.contains("supplierId")) setSupplierId(map["supplierId"].toString());
    if (map.contains("materialDate")) setMaterialDate(map["materialDate"].toDate());
    if (map.contains("notes")) setNotes(map["notes"].toString());
}
