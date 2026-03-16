#include "cameracontroller.h"
#include <Qt3DRender/QCamera>
#include <Qt3DInput/QMouseEvent>
#include <Qt3DInput/QKeyEvent>
#include <Qt3DInput/QWheelEvent>
#include <QDebug>
#include <QtMath>

CameraController::CameraController(Qt3DCore::QEntity *parent)
    : Qt3DCore::QEntity(parent)
{
}

void CameraController::setCamera(Qt3DRender::QCamera *camera)
{
    if (m_camera != camera) {
        m_camera = camera;
        
        if (m_camera) {
            // Initialize position based on camera
            QVector3D pos = m_camera->position();
            QVector3D target = m_camera->viewCenter();
            
            m_targetPosition = target;
            QVector3D diff = pos - target;
            m_distance = diff.length();
            
            // Calculate azimuth and elevation
            m_azimuth = qRadiansToDegrees(qAtan2(diff.x(), diff.z()));
            m_elevation = qRadiansToDegrees(qAsin(diff.y() / m_distance));
        }
        
        emit cameraChanged();
    }
}

void CameraController::setLinearSpeed(float speed)
{
    if (!qFuzzyCompare(m_linearSpeed, speed)) {
        m_linearSpeed = speed;
        emit linearSpeedChanged();
    }
}

void CameraController::setLookSpeed(float speed)
{
    if (!qFuzzyCompare(m_lookSpeed, speed)) {
        m_lookSpeed = speed;
        emit lookSpeedChanged();
    }
}

void CameraController::mousePressEvent(QMouseEvent *event)
{
    switch (event->button()) {
        case Qt::LeftButton:
            m_leftMousePressed = true;
            break;
        case Qt::RightButton:
            m_rightMousePressed = true;
            break;
        case Qt::MiddleButton:
            m_middleMousePressed = true;
            break;
        default:
            break;
    }
    m_lastMousePos = event->pos();
}

void CameraController::mouseReleaseEvent(QMouseEvent *event)
{
    switch (event->button()) {
        case Qt::LeftButton:
            m_leftMousePressed = false;
            break;
        case Qt::RightButton:
            m_rightMousePressed = false;
            break;
        case Qt::MiddleButton:
            m_middleMousePressed = false;
            break;
        default:
            break;
    }
}

void CameraController::mouseMoveEvent(QMouseEvent *event)
{
    QPoint delta = event->pos() - m_lastMousePos;
    m_lastMousePos = event->pos();
    
    if (!m_camera) return;
    
    if (m_leftMousePressed) {
        // Orbit camera
        rotateCamera(delta.x(), delta.y());
    } else if (m_middleMousePressed || (m_leftMousePressed && m_rightMousePressed)) {
        // Pan camera
        panCamera(delta.x(), delta.y());
    }
}

void CameraController::wheelEvent(QWheelEvent *event)
{
    if (!m_camera) return;
    
    float delta = event->angleDelta().y() / 120.0f;
    zoomCamera(delta);
}

void CameraController::keyPressEvent(QKeyEvent *event)
{
    if (!m_camera) return;
    
    float moveSpeed = m_linearSpeed * 0.1f;
    
    switch (event->key()) {
        case Qt::Key_W:
        case Qt::Key_Up:
            m_targetPosition += m_camera->viewVector().normalized() * moveSpeed;
            break;
        case Qt::Key_S:
        case Qt::Key_Down:
            m_targetPosition -= m_camera->viewVector().normalized() * moveSpeed;
            break;
        case Qt::Key_A:
        case Qt::Key_Left:
            m_targetPosition -= m_camera->rightVector().normalized() * moveSpeed;
            break;
        case Qt::Key_D:
        case Qt::Key_Right:
            m_targetPosition += m_camera->rightVector().normalized() * moveSpeed;
            break;
        case Qt::Key_Q:
            m_targetPosition += QVector3D(0, moveSpeed, 0);
            break;
        case Qt::Key_E:
            m_targetPosition -= QVector3D(0, moveSpeed, 0);
            break;
        default:
            break;
    }
    
    updateCameraPosition();
}

void CameraController::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event)
}

void CameraController::updateCameraPosition()
{
    if (!m_camera) return;
    
    // Calculate camera position from spherical coordinates
    float azimuthRad = qDegreesToRadians(m_azimuth);
    float elevationRad = qDegreesToRadians(m_elevation);
    
    float x = m_distance * qCos(elevationRad) * qSin(azimuthRad);
    float y = m_distance * qSin(elevationRad);
    float z = m_distance * qCos(elevationRad) * qCos(azimuthRad);
    
    QVector3D cameraPos = m_targetPosition + QVector3D(x, y, z);
    
    m_camera->setPosition(cameraPos);
    m_camera->setViewCenter(m_targetPosition);
}

void CameraController::rotateCamera(float dx, float dy)
{
    m_azimuth += dx * m_lookSpeed * 0.01f;
    m_elevation += dy * m_lookSpeed * 0.01f;
    
    // Clamp elevation
    m_elevation = qBound(-89.0f, m_elevation, 89.0f);
    
    updateCameraPosition();
}

void CameraController::zoomCamera(float delta)
{
    m_distance *= qPow(0.9f, delta);
    m_distance = qBound(1.0f, m_distance, 500.0f);
    
    updateCameraPosition();
}

void CameraController::panCamera(float dx, float dy)
{
    float panSpeed = m_distance * 0.001f;
    
    QVector3D right = m_camera->rightVector().normalized();
    QVector3D up = m_camera->upVector().normalized();
    
    m_targetPosition -= right * dx * panSpeed;
    m_targetPosition += up * dy * panSpeed;
    
    updateCameraPosition();
}
