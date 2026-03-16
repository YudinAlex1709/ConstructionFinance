#ifndef SCENE3D_H
#define SCENE3D_H

#include <QObject>
#include <Qt3DCore/QEntity>
#include <Qt3DRender/QCamera>
#include <QColor>
#include <QVector3D>

namespace Qt3DCore {
    class QEntity;
}

namespace Qt3DRender {
    class QCamera;
}

class BuildingModel;

class Scene3D : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QObject* rootEntity READ rootEntity CONSTANT)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(int viewMode READ viewMode WRITE setViewMode NOTIFY viewModeChanged)

public:
    enum ViewMode {
        Wireframe = 0,
        Solid = 1,
        Textured = 2
    };
    Q_ENUM(ViewMode)

    explicit Scene3D(QObject *parent = nullptr);
    ~Scene3D();

    QObject* rootEntity() const { return m_rootEntity; }
    bool isLoading() const { return m_isLoading; }
    int viewMode() const { return m_viewMode; }
    void setViewMode(int mode);

    Q_INVOKABLE void loadBuildingModel(const QString &filePath);
    Q_INVOKABLE void clearScene();
    Q_INVOKABLE void zoomToFit();
    Q_INVOKABLE void highlightStage(const QString &stageName);
    Q_INVOKABLE void setStageColor(const QString &stageName, const QColor &color);
    Q_INVOKABLE void showFinancialHeatmap(const QVariantMap &stageCosts);
    Q_INVOKABLE void clearHeatmap();
    
    Q_INVOKABLE void createProceduralBuilding(int floors, double width, double depth, double floorHeight);
    
    Q_INVOKABLE void setCameraPosition(const QVector3D &position);
    Q_INVOKABLE void setCameraTarget(const QVector3D &target);
    Q_INVOKABLE QVector3D screenToWorld(int x, int y);

signals:
    void isLoadingChanged();
    void viewModeChanged();
    void modelLoaded();
    void error(const QString &message);
    void stageClicked(const QString &stageName);
    void stageHovered(const QString &stageName);

private:
    void setupScene();
    void setupLighting();
    void createGroundPlane();
    void createAxesHelper();
    
    Qt3DCore::QEntity *m_rootEntity = nullptr;
    Qt3DRender::QCamera *m_camera = nullptr;
    BuildingModel *m_currentBuilding = nullptr;
    bool m_isLoading = false;
    int m_viewMode = Solid;
};

#endif // SCENE3D_H
