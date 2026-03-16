#ifndef CAMERACONTROLLER_H
#define CAMERACONTROLLER_H

#include <Qt3DCore/QEntity>
#include <QVector3D>
#include <QPoint>

namespace Qt3DRender {
    class QCamera;
}

class CameraController : public Qt3DCore::QEntity
{
    Q_OBJECT
    Q_PROPERTY(Qt3DRender::QCamera* camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(float linearSpeed READ linearSpeed WRITE setLinearSpeed NOTIFY linearSpeedChanged)
    Q_PROPERTY(float lookSpeed READ lookSpeed WRITE setLookSpeed NOTIFY lookSpeedChanged)

public:
    explicit CameraController(Qt3DCore::QEntity *parent = nullptr);
    
    Qt3DRender::QCamera* camera() const { return m_camera; }
    void setCamera(Qt3DRender::QCamera *camera);
    
    float linearSpeed() const { return m_linearSpeed; }
    void setLinearSpeed(float speed);
    
    float lookSpeed() const { return m_lookSpeed; }
    void setLookSpeed(float speed);

signals:
    void cameraChanged();
    void linearSpeedChanged();
    void lookSpeedChanged();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

private:
    void updateCameraPosition();
    void rotateCamera(float dx, float dy);
    void zoomCamera(float delta);
    void panCamera(float dx, float dy);
    
    Qt3DRender::QCamera *m_camera = nullptr;
    float m_linearSpeed = 10.0f;
    float m_lookSpeed = 180.0f;
    
    bool m_leftMousePressed = false;
    bool m_rightMousePressed = false;
    bool m_middleMousePressed = false;
    QPoint m_lastMousePos;
    
    QVector3D m_targetPosition;
    float m_distance = 50.0f;
    float m_azimuth = 45.0f;
    float m_elevation = 30.0f;
};

#endif // CAMERACONTROLLER_H
