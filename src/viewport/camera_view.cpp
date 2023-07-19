
#include <QLabel>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>

#include "viewport/camera_view.hpp"

#include "project/transform_component.hpp"

namespace g::viewport
{

CameraView::CameraView(QWidget* parent)
    : Viewport(parent)
{
    QSurfaceFormat format;
    format.setSamples(16);
    setFormat(format);

    _label = new QLabel(this);
    _label->move(0, 0);
    _label->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

CameraView::~CameraView() = default;

void CameraView::setCamera(std::shared_ptr<project::camera_component> camera)
{
    _camera = camera;

    _projection = common::matrix4x4f::perspective(
        45.0f, width() / height(), 0.1f, 100.0f);

    recalculateViewMatrix();
    update();
}

std::shared_ptr<project::camera_component> CameraView::camera() const
{
    return _camera;
}

void CameraView::initializeGL() { Viewport::initializeGL(); }

void CameraView::resizeGL(int w, int h)
{
    _projection = common::matrix4x4f::perspective(
        20.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);

    _label->resize(w, h);

    recalculateViewMatrix();

    Viewport::resizeGL(w, h);
}

void CameraView::paintGL() { Viewport::paintGL(); }

void CameraView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        _last_mouse_pos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    update();
}

void CameraView::mouseMoveEvent(QMouseEvent* event) { }

void CameraView::mouseReleaseEvent(QMouseEvent* event) { }

void CameraView::wheelEvent(QWheelEvent* event) { }

void CameraView::recalculateViewMatrix()
{
    QMatrix4x4 view;
    view.setToIdentity();

    auto position = _camera->object()->transform()->position();
    auto rotation = _camera->object()->transform()->rotation();

    QVector3D qposition(position.x(), position.y(), position.z());
    QQuaternion qrotation(rotation.w, rotation.x, rotation.y, rotation.z);

    // view.lookAt(qposition, qposition + forward, up);
    view.lookAt(qposition, { 0, 0, 0 }, { 0, 1, 0 });
    _view = common::matrix4x4f::from_raw_data(view.data());
}

} // namespace g::viewport
