#ifndef BUILDINGMODEL_H
#define BUILDINGMODEL_H

#include <Qt3DCore/QEntity>
#include <QColor>
#include <QVector3D>
#include <QVariantMap>

namespace Qt3DCore {
    class QTransform;
}

namespace Qt3DExtras {
    class QPhongMaterial;
    class QCuboidMesh;
}

class BuildingStage : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(double budget READ budget WRITE setBudget NOTIFY budgetChanged)
    Q_PROPERTY(double spent READ spent WRITE setSpent NOTIFY spentChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool isHighlighted READ isHighlighted WRITE setIsHighlighted NOTIFY isHighlightedChanged)

public:
    explicit BuildingStage(QObject *parent = nullptr);
    
    QString name() const { return m_name; }
    void setName(const QString &name);
    
    double budget() const { return m_budget; }
    void setBudget(double budget);
    
    double spent() const { return m_spent; }
    void setSpent(double spent);
    
    QColor color() const { return m_color; }
    void setColor(const QColor &color);
    
    bool isHighlighted() const { return m_isHighlighted; }
    void setIsHighlighted(bool highlighted);
    
    Qt3DCore::QEntity* entity() const { return m_entity; }
    void setEntity(Qt3DCore::QEntity *entity);

signals:
    void nameChanged();
    void budgetChanged();
    void spentChanged();
    void colorChanged();
    void isHighlightedChanged();
    void clicked();

private:
    QString m_name;
    double m_budget = 0.0;
    double m_spent = 0.0;
    QColor m_color = Qt::gray;
    bool m_isHighlighted = false;
    Qt3DCore::QEntity *m_entity = nullptr;
};

class BuildingModel : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(QVariantList stages READ stagesVariant NOTIFY stagesChanged)
    Q_PROPERTY(QString projectId READ projectId WRITE setProjectId NOTIFY projectIdChanged)
    Q_PROPERTY(double totalHeight READ totalHeight NOTIFY totalHeightChanged)
    Q_PROPERTY(QVector3D dimensions READ dimensions NOTIFY dimensionsChanged)

public:
    explicit BuildingModel(Qt3DCore::QEntity *parent = nullptr);
    ~BuildingModel();
    
    QString projectId() const { return m_projectId; }
    void setProjectId(const QString &id);
    
    QVariantList stagesVariant() const;
    QList<BuildingStage*> stages() const { return m_stages; }
    
    double totalHeight() const { return m_totalHeight; }
    QVector3D dimensions() const { return m_dimensions; }

    Q_INVOKABLE void loadFromIFC(const QString &filePath);
    Q_INVOKABLE void loadFromJSON(const QString &filePath);
    Q_INVOKABLE void createProceduralBuilding(int floors, double width, double depth, double floorHeight);
    
    Q_INVOKABLE void updateStageFinancials(const QString &stageName, double budget, double spent);
    Q_INVOKABLE void applyHeatmap();
    Q_INVOKABLE void clearHeatmap();
    
    Q_INVOKABLE BuildingStage* stageAt(const QVector3D &position);
    Q_INVOKABLE void highlightStage(const QString &stageName);
    Q_INVOKABLE void setStageVisibility(const QString &stageName, bool visible);

signals:
    void stagesChanged();
    void projectIdChanged();
    void totalHeightChanged();
    void dimensionsChanged();
    void stageSelected(BuildingStage *stage);
    void stageClicked(const QString &stageName);

private:
    void addStage(const QString &name, const QVector3D &position, 
                  const QVector3D &size, const QColor &color);
    void createFloor(int floorNumber, double y, double width, double depth, double height);
    void createFoundation(double width, double depth);
    void createRoof(double y, double width, double depth);
    
    QColor calculateHeatmapColor(double spent, double budget);
    void updateMaterials();
    void clearStages();
    
    QString m_projectId;
    QList<BuildingStage*> m_stages;
    Qt3DCore::QEntity *m_stagesContainer = nullptr;
    double m_totalHeight = 0.0;
    QVector3D m_dimensions;
    bool m_heatmapActive = false;
};

#endif // BUILDINGMODEL_H
