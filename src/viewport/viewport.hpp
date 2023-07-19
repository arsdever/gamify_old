#pragma once

#include <QOpenGLWidget>

#include "common/matrix.hpp"

class QOpenGLFunctions;
class QOpenGLFunctions_3_3_Core;
class QLibrary;

namespace g::rendering
{
class renderer;
} // namespace g::rendering

namespace g::project
{
class scene;
class renderer_component;
} // namespace g::project

namespace g::viewport
{

class Viewport : public QOpenGLWidget
{
    Q_OBJECT
public:
    Viewport(QWidget* parent = nullptr);
    ~Viewport() override;

    void onInitialized(std::function<void()> onInitialized);

    void draw(std::shared_ptr<project::renderer_component> rendererComponent);
    void loadScene(std::shared_ptr<project::scene> scene);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

private:
    QOpenGLFunctions_3_3_Core* checkAndGetGLFunctions();

protected:
    QLibrary* _renderer_lib;
    std::unique_ptr<rendering::renderer, void (*)(rendering::renderer*)>
        _renderer;
    std::shared_ptr<project::scene> _scene;
    std::function<void()> _onInitialized;

    common::matrix4x4f _projection;
    common::matrix4x4f _view;
};

} // namespace g::viewport
