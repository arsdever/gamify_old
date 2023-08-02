#pragma once

#include <QWindow>

#include "common/matrix.hpp"

class QOpenGLContext;
class QLibrary;

namespace g::rendering
{
class renderer;
} // namespace g::rendering

namespace g::project
{
class scene;
class renderer_component;
class camera_component;
} // namespace g::project

namespace g::viewport
{

class Viewport : public QWindow
{
    Q_OBJECT
public:
    Viewport();
    ~Viewport() override;

    void onInitialized(std::function<void()> onInitialized);

    void draw(std::shared_ptr<project::renderer_component> rendererComponent);
    void loadScene(std::shared_ptr<project::scene> scene);

    void setCamera(std::shared_ptr<project::camera_component> camera);
    std::shared_ptr<project::camera_component> camera() const;

protected:
    bool event(QEvent* event) override;

private:
    void initialize();
    void render() ;
    void renderLater();
    void recalculateViewMatrix();

protected:
    QLibrary* _renderer_lib = nullptr;
    QOpenGLContext* _context = nullptr;

    std::shared_ptr<project::camera_component> _camera;
    std::unique_ptr<rendering::renderer, void (*)(rendering::renderer*)>
        _renderer;
    std::shared_ptr<project::scene> _scene;
    std::function<void()> _onInitialized;
    bool _needsInitialize = true;

    common::matrix4x4f _projection;
    common::matrix4x4f _view;

    QPoint _last_mouse_pos;
    // QLabel* _label;
};

} // namespace g::viewport
