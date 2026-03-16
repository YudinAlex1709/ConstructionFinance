#include "category.h"
#include "../database/database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Category::Category(QObject *parent)
    : BaseModel(parent)
{
}

void Category::setCompanyId(const QString &id)
{
    if (m_companyId != id) {
        m_companyId = id;
        emit companyIdChanged();
        setIsDirty(true);
    }
}

void Category::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
        setIsDirty(true);
    }
}

void Category::setType(CategoryType type)
{
    if (m_type != type) {
        m_type = type;
        emit typeChanged();
        setIsDirty(true);
    }
}

void Category::setColor(const QString &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
        setIsDirty(true);
    }
}

void Category::setIcon(const QString &icon)
{
    if (m_icon != icon) {
        m_icon = icon;
        emit iconChanged();
        setIsDirty(true);
    }
}

void Category::setParentId(const QString &id)
{
    if (m_parentId != id) {
        m_parentId = id;
        emit parentIdChanged();
        setIsDirty(true);
    }
}

void Category::setIsSystem(bool isSystem)
{
    if (m_isSystem != isSystem) {
        m_isSystem = isSystem;
        emit isSystemChanged();
        setIsDirty(true);
    }
}

QString Category::typeLabel() const
{
    return m_type == Income ? tr("Доход") : tr("Расход");
}

bool Category::save()
{
    if (m_id.isEmpty()) {
        generateId();
    }
    
    QSqlQuery query;
    query.prepare(R"(
        INSERT OR REPLACE INTO categories 
        (id, company_id, name, type, color, icon, parent_id, is_system)
        VALUES 
        (:id, :company_id, :name, :type, :color, :icon, :parent_id, :is_system)
    )");
    
    query.bindValue(":id", m_id);
    query.bindValue(":company_id", m_companyId);
    query.bindValue(":name", m_name);
    query.bindValue(":type", static_cast<int>(m_type));
    query.bindValue(":color", m_color);
    query.bindValue(":icon", m_icon);
    query.bindValue(":parent_id", m_parentId);
    query.bindValue(":is_system", m_isSystem ? 1 : 0);
    
    if (!query.exec()) {
        qWarning() << "Failed to save category:" << query.lastError().text();
        emit error(tr("Ошибка сохранения категории: %1").arg(query.lastError().text()));
        return false;
    }
    
    setIsDirty(false);
    emit saved();
    return true;
}

bool Category::remove()
{
    if (m_id.isEmpty() || m_isSystem) return false;
    
    QSqlQuery query;
    query.prepare("DELETE FROM categories WHERE id = :id");
    query.bindValue(":id", m_id);
    
    if (!query.exec()) {
        qWarning() << "Failed to remove category:" << query.lastError().text();
        emit error(tr("Ошибка удаления категории: %1").arg(query.lastError().text()));
        return false;
    }
    
    emit removed();
    return true;
}

void Category::loadFromRecord(const QSqlRecord &record)
{
    setId(record.value("id").toString());
    setCompanyId(record.value("company_id").toString());
    setName(record.value("name").toString());
    setType(static_cast<CategoryType>(record.value("type").toInt()));
    setColor(record.value("color").toString());
    setIcon(record.value("icon").toString());
    setParentId(record.value("parent_id").toString());
    setIsSystem(record.value("is_system").toBool());
    setIsDirty(false);
}

QVariantMap Category::toMap() const
{
    QVariantMap map;
    map["id"] = m_id;
    map["companyId"] = m_companyId;
    map["name"] = m_name;
    map["type"] = static_cast<int>(m_type);
    map["typeLabel"] = typeLabel();
    map["color"] = m_color;
    map["icon"] = m_icon;
    map["parentId"] = m_parentId;
    map["isSystem"] = m_isSystem;
    return map;
}

void Category::fromMap(const QVariantMap &map)
{
    if (map.contains("id")) setId(map["id"].toString());
    if (map.contains("companyId")) setCompanyId(map["companyId"].toString());
    if (map.contains("name")) setName(map["name"].toString());
    if (map.contains("type")) setType(static_cast<CategoryType>(map["type"].toInt()));
    if (map.contains("color")) setColor(map["color"].toString());
    if (map.contains("icon")) setIcon(map["icon"].toString());
    if (map.contains("parentId")) setParentId(map["parentId"].toString());
    if (map.contains("isSystem")) setIsSystem(map["isSystem"].toBool());
}
