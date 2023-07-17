#include <QLabel>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>

#include "viewport/camera_view.hpp"

#include "common/quaternion.hpp"
#include "project/transform_component.hpp"

namespace g::viewport
{

CameraView::CameraView(QWidget* parent)
    : Viewport(parent)
{
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);
}

CameraView::~CameraView() = default;

void CameraView::setCamera(std::shared_ptr<project::camera_component> camera)
{
    _camera = camera;

    QMatrix4x4 projection;
    QMatrix4x4 view;

    projection.setToIdentity();
    projection.perspective(45.0f, width() / height(), 0.1f, 100.0f);

    view.setToIdentity();
    auto position = camera->object()->transform()->position();
    view.lookAt(QVector3D(static_cast<float>(position.x),
                          static_cast<float>(position.y),
                          static_cast<float>(position.z)),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0));

    std::copy(
        projection.data(), projection.data() + 16, _projection.data.data());
    std::copy(view.data(), view.data() + 16, _view.data.data());
}

std::shared_ptr<project::camera_component> CameraView::camera() const
{
    return _camera;
}

void CameraView::initializeGL() { Viewport::initializeGL(); }

void CameraView::resizeGL(int w, int h)
{
    QMatrix4x4 projection;

    projection.setToIdentity();
    projection.perspective(
        20.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);
    std::copy(
        projection.data(), projection.data() + 16, _projection.data.data());

    recalculateViewMatrix();

    Viewport::resizeGL(w, h);
}

void CameraView::paintGL() { Viewport::paintGL(); }

void CameraView::mousePressEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MouseButton::MiddleButton)
    {
        _last_mouse_pos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    update();
}

void CameraView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MouseButton::MiddleButton)
    {
        auto delta = event->pos() - _last_mouse_pos;
        _last_mouse_pos = event->pos();

        if (event->modifiers() & Qt::AltModifier)
        {
            auto transform = _camera->object()->transform();
            QVector3D currentPosition = { transform->position().x,
                                          transform->position().y,
                                          transform->position().z };
            QQuaternion currentRotation = { transform->rotation().w,
                                            transform->rotation().x,
                                            transform->rotation().y,
                                            transform->rotation().z };

            auto rotatedPosition = QQuaternion::fromAxisAndAngle(
                                       currentRotation * QVector3D { 0, 1, 0 },
                                       delta.x() * 0.01f) *
                                   currentPosition;
            rotatedPosition = QQuaternion::fromAxisAndAngle(
                                  currentRotation * QVector3D { 1, 0, 0 },
                                  delta.y() * 0.01f) *
                              rotatedPosition;

            currentRotation *= QQuaternion::fromAxisAndAngle(
                currentRotation * QVector3D { 0, 1, 0 }, delta.x() * 0.1f);
            currentRotation *= QQuaternion::fromAxisAndAngle(
                currentRotation * QVector3D { 1, 0, 0 }, delta.y() * 0.1f);
            currentRotation.normalize();
            transform->set_position({ rotatedPosition.x(),
                                      rotatedPosition.y(),
                                      rotatedPosition.z() });
            transform->set_rotation({ currentRotation.x(),
                                      currentRotation.y(),
                                      currentRotation.z(),
                                      currentRotation.scalar() });
        }
        else
        {
            auto transform = _camera->object()->transform();
            QQuaternion rotation { transform->rotation().w,
                                   transform->rotation().x,
                                   transform->rotation().y,
                                   transform->rotation().z };
            QVector3D x =
                rotation.rotatedVector(QVector3D(1, 0, 0)).normalized();
            QVector3D y =
                rotation.rotatedVector(QVector3D(0, 1, 0)).normalized();

            x *= delta.x() * 0.01f;
            y *= delta.y() * 0.01f;

            transform->set_position(
                common::vector3 { transform->position().x + x.x() + y.x(),
                                  transform->position().y + x.y() + y.y(),
                                  transform->position().z + x.z() + y.z() });
        }
        recalculateViewMatrix();
        update();
    }
}

void CameraView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        setCursor(Qt::ArrowCursor);
    }
}

void CameraView::wheelEvent(QWheelEvent* event)
{
    auto transform = _camera->object()->transform();
    QQuaternion rotation { transform->rotation().w,
                           transform->rotation().x,
                           transform->rotation().y,
                           transform->rotation().z };
    QVector3D forward =
        rotation.rotatedVector(QVector3D(0, 0, -1)).normalized();
    transform->set_position(
        common::vector3 { transform->position().x + forward.x() * 0.1f,
                          transform->position().y + forward.y() * 0.1f,
                          transform->position().z + forward.z() * 0.1f });

    update();
}

void CameraView::recalculateViewMatrix()
{
    QMatrix4x4 view;
    view.setToIdentity();

    auto position = _camera->object()->transform()->position();
    auto rotation = _camera->object()->transform()->rotation();

    QVector3D qposition(position.x, position.y, position.z);
    QQuaternion qrotation(rotation.w, rotation.x, rotation.y, rotation.z);
    QVector3D forward =
        qrotation.rotatedVector(QVector3D(0, 0, 1)).normalized();
    QVector3D up = qrotation.rotatedVector(QVector3D(0, 1, 0)).normalized();

    view.lookAt(qposition, qposition + forward, up);
    std::copy(view.data(), view.data() + 16, _view.data.data());
}

} // namespace g::viewport
