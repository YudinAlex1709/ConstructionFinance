#include "scene3d.h"
#include "buildingmodel.h"
#include <Qt3DCore/QEntity>
#include <Qt3DCore/QTransform>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QPointLight>
#include <Qt3DRender/QDirectionalLight>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QDiffuseSpecularMaterial>
#include <Qt3DExtras/QCuboidMesh>
#include <Qt3DExtras/QPlaneMesh>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DExtras/QFirstPersonCameraController>
#include <QDebug>

Scene3D::Scene3D(QObject *parent)
    : QObject(parent)
{
    setupScene();
}

Scene3D::~Scene3D()
{
    if (m_rootEntity) {
        delete m_rootEntity;
    }
}

void Scene3D::setViewMode(int mode)
{
    if (m_viewMode != mode) {
        m_viewMode = mode;
        emit viewModeChanged();
        
        // Update materials based on view mode
        if (m_currentBuilding) {
            // TODO: Update building materials
        }
    }
}

void Scene3D::setupScene()
{
    m_rootEntity = new Qt3DCore::QEntity();
    
    // Create camera
    m_camera = new Qt3DRender::QCamera(m_rootEntity);
    m_camera->setProjectionType(Qt3DRender::QCameraLens::PerspectiveProjection);
    m_camera->setFieldOfView(45.0f);
    m_camera->setNearPlane(0.1f);
    m_camera->setFarPlane(1000.0f);
    m_camera->setPosition(QVector3D(50.0f, 50.0f, 50.0f));
    m_camera->setViewCenter(QVector3D(0.0f, 10.0f, 0.0f));
    m_camera->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    
    // Create camera controller
    auto *camController = new Qt3DExtras::QOrbitCameraController(m_rootEntity);
    camController->setCamera(m_camera);
    camController->setLookSpeed(180.0f);
    camController->setLinearSpeed(15.0f);
    
    setupLighting();
    createGroundPlane();
    createAxesHelper();
}

void Scene3D::setupLighting()
{
    // Ambient light
    auto *ambientLight = new Qt3DRender::QDirectionalLight(m_rootEntity);
    ambientLight->setColor(QColor(255, 255, 255));
    ambientLight->setIntensity(0.4f);
    ambientLight->setWorldDirection(QVector3D(-1.0f, -1.0f, -1.0f));
    
    auto *ambientLightEntity = new Qt3DCore::QEntity(m_rootEntity);
    ambientLightEntity->addComponent(ambientLight);
    
    // Main directional light
    auto *mainLight = new Qt3DRender::QDirectionalLight(m_rootEntity);
    mainLight->setColor(QColor(255, 255, 255));
    mainLight->setIntensity(0.8f);
    mainLight->setWorldDirection(QVector3D(-1.0f, -2.0f, -1.0f));
    
    auto *mainLightEntity = new Qt3DCore::QEntity(m_rootEntity);
    mainLightEntity->addComponent(mainLight);
    
    // Fill light
    auto *fillLight = new Qt3DRender::QDirectionalLight(m_rootEntity);
    fillLight->setColor(QColor(200, 220, 255));
    fillLight->setIntensity(0.3f);
    fillLight->setWorldDirection(QVector3D(1.0f, -0.5f, 1.0f));
    
    auto *fillLightEntity = new Qt3DCore::QEntity(m_rootEntity);
    fillLightEntity->addComponent(fillLight);
}

void Scene3D::createGroundPlane()
{
    auto *groundEntity = new Qt3DCore::QEntity(m_rootEntity);
    
    auto *mesh = new Qt3DExtras::QPlaneMesh(groundEntity);
    mesh->setWidth(200.0f);
    mesh->setHeight(200.0f);
    
    auto *material = new Qt3DExtras::QPhongMaterial(groundEntity);
    material->setDiffuse(QColor(240, 240, 240));
    material->setAmbient(QColor(200, 200, 200));
    material->setShininess(0.0f);
    
    auto *transform = new Qt3DCore::QTransform(groundEntity);
    transform->setRotation(QQuaternion::fromAxisAndAngle(QVector3D(1.0f, 0.0f, 0.0f), -90.0f));
    
    groundEntity->addComponent(mesh);
    groundEntity->addComponent(material);
    groundEntity->addComponent(transform);
}

void Scene3D::createAxesHelper()
{
    // X axis (red)
    auto *xAxis = new Qt3DCore::QEntity(m_rootEntity);
    auto *xMesh = new Qt3DExtras::QCuboidMesh(xAxis);
    xMesh->setXExtent(10.0f);
    xMesh->setYExtent(0.1f);
    xMesh->setZExtent(0.1f);
    auto *xMat = new Qt3DExtras::QPhongMaterial(xAxis);
    xMat->setDiffuse(QColor(255, 0, 0));
    auto *xTrans = new Qt3DCore::QTransform(xAxis);
    xTrans->setTranslation(QVector3D(5.0f, 0.05f, 0.0f));
    xAxis->addComponent(xMesh);
    xAxis->addComponent(xMat);
    xAxis->addComponent(xTrans);
    
    // Y axis (green)
    auto *yAxis = new Qt3DCore::QEntity(m_rootEntity);
    auto *yMesh = new Qt3DExtras::QCuboidMesh(yAxis);
    yMesh->setXExtent(0.1f);
    yMesh->setYExtent(10.0f);
    yMesh->setZExtent(0.1f);
    auto *yMat = new Qt3DExtras::QPhongMaterial(yAxis);
    yMat->setDiffuse(QColor(0, 255, 0));
    auto *yTrans = new Qt3DCore::QTransform(yAxis);
    yTrans->setTranslation(QVector3D(0.0f, 5.0f, 0.0f));
    yAxis->addComponent(yMesh);
    yAxis->addComponent(yMat);
    yAxis->addComponent(yTrans);
    
    // Z axis (blue)
    auto *zAxis = new Qt3DCore::QEntity(m_rootEntity);
    auto *zMesh = new Qt3DExtras::QCuboidMesh(zAxis);
    zMesh->setXExtent(0.1f);
    zMesh->setYExtent(0.1f);
    zMesh->setZExtent(10.0f);
    auto *zMat = new Qt3DExtras::QPhongMaterial(zAxis);
    zMat->setDiffuse(QColor(0, 0, 255));
    auto *zTrans = new Qt3DCore::QTransform(zAxis);
    zTrans->setTranslation(QVector3D(0.0f, 0.05f, 5.0f));
    zAxis->addComponent(zMesh);
    zAxis->addComponent(zMat);
    zAxis->addComponent(zTrans);
}

void Scene3D::loadBuildingModel(const QString &filePath)
{
    m_isLoading = true;
    emit isLoadingChanged();
    
    clearScene();
    
    m_currentBuilding = new BuildingModel(m_rootEntity);
    m_currentBuilding->loadFromJSON(filePath);
    
    m_isLoading = false;
    emit isLoadingChanged();
    emit modelLoaded();
}

void Scene3D::clearScene()
{
    if (m_currentBuilding) {
        delete m_currentBuilding;
        m_currentBuilding = nullptr;
    }
}

void Scene3D::zoomToFit()
{
    if (!m_currentBuilding) return;
    
    // TODO: Calculate bounding box and set camera position
    m_camera->setPosition(QVector3D(50.0f, 50.0f, 50.0f));
    m_camera->setViewCenter(QVector3D(0.0f, 10.0f, 0.0f));
}

void Scene3D::highlightStage(const QString &stageName)
{
    if (!m_currentBuilding) return;
    // TODO: Implement stage highlighting
}

void Scene3D::setStageColor(const QString &stageName, const QColor &color)
{
    if (!m_currentBuilding) return;
    // TODO: Implement stage color change
}

void Scene3D::showFinancialHeatmap(const QVariantMap &stageCosts)
{
    if (!m_currentBuilding) return;
    // TODO: Implement heatmap visualization
}

void Scene3D::clearHeatmap()
{
    if (!m_currentBuilding) return;
    // TODO: Clear heatmap
}

void Scene3D::createProceduralBuilding(int floors, double width, double depth, double floorHeight)
{
    clearScene();
    
    m_currentBuilding = new BuildingModel(m_rootEntity);
    m_currentBuilding->createProceduralBuilding(floors, width, depth, floorHeight);
    
    emit modelLoaded();
}

void Scene3D::setCameraPosition(const QVector3D &position)
{
    if (m_camera) {
        m_camera->setPosition(position);
    }
}

void Scene3D::setCameraTarget(const QVector3D &target)
{
    if (m_camera) {
        m_camera->setViewCenter(target);
    }
}

QVector3D Scene3D::screenToWorld(int x, int y)
{
    // TODO: Implement screen to world coordinate conversion
    return QVector3D();
}
