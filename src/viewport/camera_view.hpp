#pragma once

#include "project/camera_component.hpp"
#include "viewport/viewport.hpp"

class QLabel;

namespace g::viewport
{

class CameraView : public Viewport
{
public:
    CameraView(QWidget* parent = nullptr);
    ~CameraView();

    void setCamera(std::shared_ptr<project::camera_component> camera);
    std::shared_ptr<project::camera_component> camera() const;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

private:
    std::shared_ptr<project::camera_component> _camera;

    QPoint _last_mouse_pos;
    QLabel* _label;
};

} // namespace g::viewport
