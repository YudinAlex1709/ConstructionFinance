#include "buildingmodel.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QCylinderMesh>
#include <Qt3DExtras/QConeMesh>
#include <Qt3DRender/QObjectPicker>
#include <Qt3DRender/QPickEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>

BuildingStage::BuildingStage(QObject *parent)
    : QObject(parent)
{
}

void BuildingStage::setName(const QString &name)
{
    if (m_name != name) {
        m_name = name;
        emit nameChanged();
    }
}

void BuildingStage::setBudget(double budget)
{
    if (!qFuzzyCompare(m_budget, budget)) {
        m_budget = budget;
        emit budgetChanged();
    }
}

void BuildingStage::setSpent(double spent)
{
    if (!qFuzzyCompare(m_spent, spent)) {
        m_spent = spent;
        emit spentChanged();
    }
}

void BuildingStage::setColor(const QColor &color)
{
    if (m_color != color) {
        m_color = color;
        emit colorChanged();
    }
}

void BuildingStage::setIsHighlighted(bool highlighted)
{
    if (m_isHighlighted != highlighted) {
        m_isHighlighted = highlighted;
        emit isHighlightedChanged();
    }
}

void BuildingStage::setEntity(Qt3DCore::QEntity *entity)
{
    m_entity = entity;
}

// BuildingModel implementation

BuildingModel::BuildingModel(Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
    , m_stagesContainer(new Qt3DCore::QEntity(this))
{
}

BuildingModel::~BuildingModel()
{
    clearStages();
}

void BuildingModel::setProjectId(const QString &id)
{
    if (m_projectId != id) {
        m_projectId = id;
        emit projectIdChanged();
    }
}

QVariantList BuildingModel::stagesVariant() const
{
    QVariantList list;
    for (BuildingStage *stage : m_stages) {
        QVariantMap map;
        map["name"] = stage->name();
        map["budget"] = stage->budget();
        map["spent"] = stage->spent();
        map["color"] = stage->color();
        map["isHighlighted"] = stage->isHighlighted();
        list.append(map);
    }
    return list;
}

void BuildingModel::clearStages()
{
    for (BuildingStage *stage : m_stages) {
        delete stage;
    }
    m_stages.clear();
    
    // Remove all children from stages container
    for (Qt3DCore::QNode *child : m_stagesContainer->childNodes()) {
        delete child;
    }
}

void BuildingModel::loadFromIFC(const QString &filePath)
{
    // TODO: Implement IFC file loading using IfcOpenShell or similar
    qDebug() << "Loading IFC from:" << filePath;
}

void BuildingModel::loadFromJSON(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << filePath;
        return;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    
    if (!doc.isObject()) {
        qWarning() << "Invalid JSON format";
        return;
    }
    
    QJsonObject root = doc.object();
    
    // Parse building data
    if (root.contains("floors")) {
        int floors = root["floors"].toInt();
        double width = root["width"].toDouble(20.0);
        double depth = root["depth"].toDouble(20.0);
        double floorHeight = root["floorHeight"].toDouble(3.0);
        
        createProceduralBuilding(floors, width, depth, floorHeight);
    }
    
    // Parse stages if present
    if (root.contains("stages")) {
        QJsonArray stagesArray = root["stages"].toArray();
        for (int i = 0; i < stagesArray.size() && i < m_stages.size(); ++i) {
            QJsonObject stageObj = stagesArray[i].toObject();
            BuildingStage *stage = m_stages[i];
            stage->setBudget(stageObj["budget"].toDouble());
            stage->setSpent(stageObj["spent"].toDouble());
        }
    }
}

void BuildingModel::createProceduralBuilding(int floors, double width, double depth, double floorHeight)
{
    clearStages();
    
    m_dimensions = QVector3D(width, floors * floorHeight, depth);
    
    // Create foundation
    createFoundation(width, depth);
    
    // Create floors
    for (int i = 0; i < floors; ++i) {
        double y = i * floorHeight + floorHeight / 2.0;
        createFloor(i + 1, y, width, depth, floorHeight);
    }
    
    // Create roof
    createRoof(floors * floorHeight, width, depth);
    
    m_totalHeight = floors * floorHeight + 1.0; // +1 for roof
    emit totalHeightChanged();
    emit dimensionsChanged();
    emit stagesChanged();
}

void BuildingModel::createFoundation(double width, double depth)
{
    auto *foundation = new Qt3DCore::QEntity(m_stagesContainer);
    
    auto *mesh = new Qt3DExtras::QCuboidMesh(foundation);
    mesh->setXExtent(width + 2.0);
    mesh->setYExtent(1.0);
    mesh->setZExtent(depth + 2.0);
    
    auto *material = new Qt3DExtras::QPhongMaterial(foundation);
    material->setDiffuse(QColor(120, 120, 120));
    material->setAmbient(QColor(80, 80, 80));
    
    auto *transform = new Qt3DCore::QTransform(foundation);
    transform->setTranslation(QVector3D(0.0f, -0.5f, 0.0f));
    
    foundation->addComponent(mesh);
    foundation->addComponent(material);
    foundation->addComponent(transform);
    
    BuildingStage *stage = new BuildingStage(this);
    stage->setName(tr("Фундамент"));
    stage->setEntity(foundation);
    m_stages.append(stage);
}

void BuildingModel::createFloor(int floorNumber, double y, double width, double depth, double height)
{
    auto *floor = new Qt3DCore::QEntity(m_stagesContainer);
    
    auto *mesh = new Qt3DExtras::QCuboidMesh(floor);
    mesh->setXExtent(width);
    mesh->setYExtent(height);
    mesh->setZExtent(depth);
    
    auto *material = new Qt3DExtras::QPhongMaterial(floor);
    // Vary color slightly per floor
    int colorValue = 200 - (floorNumber % 3) * 20;
    material->setDiffuse(QColor(colorValue, colorValue, colorValue + 10));
    material->setAmbient(QColor(colorValue - 40, colorValue - 40, colorValue - 30));
    
    auto *transform = new Qt3DCore::QTransform(floor);
    transform->setTranslation(QVector3D(0.0f, static_cast<float>(y), 0.0f));
    
    // Add picker for interaction
    auto *picker = new Qt3DRender::QObjectPicker(floor);
    picker->setHoverEnabled(true);
    connect(picker, &Qt3DRender::QObjectPicker::clicked, this, [this, floorNumber]() {
        emit stageClicked(QString("Этаж %1").arg(floorNumber));
    });
    
    floor->addComponent(mesh);
    floor->addComponent(material);
    floor->addComponent(transform);
    floor->addComponent(picker);
    
    BuildingStage *stage = new BuildingStage(this);
    stage->setName(QString("Этаж %1").arg(floorNumber));
    stage->setEntity(floor);
    m_stages.append(stage);
}

void BuildingModel::createRoof(double y, double width, double depth)
{
    auto *roof = new Qt3DCore::QEntity(m_stagesContainer);
    
    auto *mesh = new Qt3DExtras::QConeMesh(roof);
    mesh->setTopRadius(0.0f);
    mesh->setBottomRadius(static_cast<float>(qMax(width, depth) * 0.7));
    mesh->setLength(static_cast<float>(qMin(width, depth) * 0.5));
    mesh->setRings(2);
    mesh->setSlices(4);
    
    auto *material = new Qt3DExtras::QPhongMaterial(roof);
    material->setDiffuse(QColor(139, 90, 43)); // Brown roof color
    material->setAmbient(QColor(100, 60, 20));
    
    auto *transform = new Qt3DCore::QTransform(roof);
    transform->setTranslation(QVector3D(0.0f, static_cast<float>(y + mesh->length() / 2), 0.0f));
    transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(0.0f, 0.0f, 1.0f), 90.0f));
    
    roof->addComponent(mesh);
    roof->addComponent(material);
    roof->addComponent(transform);
    
    BuildingStage *stage = new BuildingStage(this);
    stage->setName(tr("Крыша"));
    stage->setEntity(roof);
    m_stages.append(stage);
}

void BuildingModel::addStage(const QString &name, const QVector3D &position, 
                             const QVector3D &size, const QColor &color)
{
    auto *entity = new Qt3DCore::QEntity(m_stagesContainer);
    
    auto *mesh = new Qt3DExtras::QCuboidMesh(entity);
    mesh->setXExtent(size.x());
    mesh->setYExtent(size.y());
    mesh->setZExtent(size.z());
    
    auto *material = new Qt3DExtras::QPhongMaterial(entity);
    material->setDiffuse(color);
    
    auto *transform = new Qt3DCore::QTransform(entity);
    transform->setTranslation(position);
    
    entity->addComponent(mesh);
    entity->addComponent(material);
    entity->addComponent(transform);
    
    BuildingStage *stage = new BuildingStage(this);
    stage->setName(name);
    stage->setColor(color);
    stage->setEntity(entity);
    m_stages.append(stage);
}

void BuildingModel::updateStageFinancials(const QString &stageName, double budget, double spent)
{
    for (BuildingStage *stage : m_stages) {
        if (stage->name() == stageName) {
            stage->setBudget(budget);
            stage->setSpent(spent);
            break;
        }
    }
}

QColor BuildingModel::calculateHeatmapColor(double spent, double budget)
{
    if (budget <= 0) return QColor(200, 200, 200);
    
    double ratio = spent / budget;
    
    if (ratio < 0.5) {
        // Green to yellow
        int green = 255;
        int red = static_cast<int>(255 * (ratio * 2));
        return QColor(red, green, 0);
    } else if (ratio < 1.0) {
        // Yellow to red
        int green = static_cast<int>(255 * (2.0 - ratio * 2));
        int red = 255;
        return QColor(red, green, 0);
    } else {
        // Over budget - dark red
        return QColor(180, 0, 0);
    }
}

void BuildingModel::applyHeatmap()
{
    m_heatmapActive = true;
    
    for (BuildingStage *stage : m_stages) {
        QColor heatColor = calculateHeatmapColor(stage->spent(), stage->budget());
        stage->setColor(heatColor);
    }
    
    updateMaterials();
}

void BuildingModel::clearHeatmap()
{
    m_heatmapActive = false;
    
    // Reset to default colors
    for (int i = 0; i < m_stages.size(); ++i) {
        BuildingStage *stage = m_stages[i];
        if (stage->name() == tr("Фундамент")) {
            stage->setColor(QColor(120, 120, 120));
        } else if (stage->name().startsWith(tr("Этаж"))) {
            int floorNum = stage->name().mid(6).toInt();
            int colorValue = 200 - (floorNum % 3) * 20;
            stage->setColor(QColor(colorValue, colorValue, colorValue + 10));
        } else if (stage->name() == tr("Крыша")) {
            stage->setColor(QColor(139, 90, 43));
        }
    }
    
    updateMaterials();
}

void BuildingModel::updateMaterials()
{
    // Update materials based on stage colors
    for (BuildingStage *stage : m_stages) {
        Qt3DCore::QEntity *entity = stage->entity();
        if (!entity) continue;
        
        // Find and update material component
        for (Qt3DCore::QComponent *component : entity->components()) {
            auto *material = qobject_cast<Qt3DExtras::QPhongMaterial*>(component);
            if (material) {
                material->setDiffuse(stage->color());
            }
        }
    }
}

BuildingStage* BuildingModel::stageAt(const QVector3D &position)
{
    // TODO: Implement ray casting or bounding box check
    Q_UNUSED(position)
    return nullptr;
}

void BuildingModel::highlightStage(const QString &stageName)
{
    for (BuildingStage *stage : m_stages) {
        stage->setIsHighlighted(stage->name() == stageName);
    }
}

void BuildingModel::setStageVisibility(const QString &stageName, bool visible)
{
    for (BuildingStage *stage : m_stages) {
        if (stage->name() == stageName) {
            Qt3DCore::QEntity *entity = stage->entity();
            if (entity) {
                entity->setEnabled(visible);
            }
            break;
        }
    }
}
