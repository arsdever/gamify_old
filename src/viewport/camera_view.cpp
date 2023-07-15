
#include <QMatrix4x4>

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
    update();
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

    Viewport::resizeGL(w, h);
}

void CameraView::paintGL() { Viewport::paintGL(); }

} // namespace g::viewport
