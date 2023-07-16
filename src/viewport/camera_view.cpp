
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
    QMatrix4x4 view;

    projection.setToIdentity();
    projection.perspective(
        20.0f, static_cast<float>(w) / static_cast<float>(h), 0.1f, 100.0f);

    view.setToIdentity();
    auto position = _camera->object()->transform()->position();

    view.lookAt(QVector3D(static_cast<float>(position.x),
                          static_cast<float>(position.y),
                          static_cast<float>(position.z)),
                QVector3D(0, 0, 0),
                QVector3D(0, 1, 0));

    std::copy(
        projection.data(), projection.data() + 16, _projection.data.data());
    std::copy(view.data(), view.data() + 16, _view.data.data());
    _label->resize(w, h);

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

void CameraView::mouseMoveEvent(QMouseEvent* event)
{
    if (event->buttons() & Qt::MouseButton::MiddleButton)
    {
        auto delta = event->pos() - _last_mouse_pos;
        _last_mouse_pos = event->pos();

        QMatrix4x4 view { _view.data.data() };
        view.translate(delta.x() / 100.0f, delta.y() / 100.0f, 0);
        std::copy(view.data(), view.data() + 16, _view.data.data());
        auto transform = _camera->object()->transform();
        transform->set_position(common::vector3 {
            view.column(3).x(), view.column(3).y(), view.column(3).z() });
        _label->setText(tr("pos: (%1,%2,%3) delta: (%4,%5)")
                            .arg(QString::number(view.column(3).x()))
                            .arg(QString::number(view.column(3).y()))
                            .arg(QString::number(view.column(3).z()))
                            .arg(QString::number(delta.x()))
                            .arg(QString::number(delta.y())));
    }
    update();
}

void CameraView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        setCursor(Qt::ArrowCursor);
    }
    update();
}

void CameraView::wheelEvent(QWheelEvent* event)
{
    auto transform = _camera->object()->transform();

    QMatrix4x4 view { _view.data.data() };
    auto delta = event->angleDelta().y() / 120.0f;
    view.translate(0, 0, delta);
    std::copy(view.data(), view.data() + 16, _view.data.data());
    transform->set_position(common::vector3 {
        view.column(3).x(), view.column(3).y(), view.column(3).z() });
    _label->setText(QString::number(view.column(3).x()) + "\n" +
                    QString::number(view.column(3).y()) + "\n" +
                    QString::number(view.column(3).z()) + "\n");
    update();
}

} // namespace g::viewport
