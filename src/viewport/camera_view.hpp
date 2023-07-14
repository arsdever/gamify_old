#pragma once

#include "project/camera_component.hpp"
#include "viewport/viewport.hpp"

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

private:
    std::shared_ptr<project::camera_component> _camera;
};

} // namespace g::viewport
