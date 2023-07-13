#pragma once

#include <QOpenGLWidget>

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
    void updateShader(std::string_view source,
                      unsigned int& shader,
                      unsigned int shaderType);

private:
    QLibrary* _renderer_lib;
    std::unique_ptr<rendering::renderer, void (*)(rendering::renderer*)>
        _renderer;
    std::shared_ptr<project::scene> _scene;
    std::function<void()> _onInitialized;
};

} // namespace g::viewport
