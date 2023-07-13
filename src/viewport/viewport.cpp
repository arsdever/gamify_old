#include <QLibrary>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLVersionFunctionsFactory>
#include <stdafx>

#include "viewport/viewport.hpp"

#include "common/logger.hpp"
#include "common/profiler.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include "rendering/renderer.hpp"

namespace g::viewport
{

namespace
{
common::logger_ptr logger = common::get_logger("viewport");
}

Viewport::Viewport(QWidget* parent)
    : QOpenGLWidget(parent)
    , _renderer_lib(nullptr)
    , _renderer(nullptr, nullptr)
{
    _renderer->setResourceManager(project::resource_manager::get());
}

Viewport::~Viewport() = default;

void Viewport::onInitialized(std::function<void()> onInitialized)
{
    _onInitialized = std::move(onInitialized);
}

void Viewport::draw(
    std::shared_ptr<project::renderer_component> rendererComponent)
{
    _renderer->render(rendererComponent);
}

void Viewport::loadScene(std::shared_ptr<project::scene> scene)
{
    _scene = scene;
    for (auto& root_object_uuid : scene->objects())
    {
        auto root_object =
            project::resource_manager::get_resource_static<project::object>(
                root_object_uuid);
        for (auto& object_uuid : root_object->children_uuid())
        {
            auto object =
                project::resource_manager::get_resource_static<project::object>(
                    object_uuid);
            auto rendererComponent =
                object->get_component<project::renderer_component>();
            if (rendererComponent)
            {
                _renderer->load_object(rendererComponent);
            }
        }
        auto rendererComponent =
            root_object->get_component<project::renderer_component>();
        if (rendererComponent)
        {
            _renderer->load_object(rendererComponent);
        }
    }
}

void Viewport::initializeGL()
{
    auto* f = checkAndGetGLFunctions();
    f->glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    _renderer_lib = new QLibrary("g_rendering.dll");
    if (!_renderer_lib->load())
    {
        logger->error("Failed to load renderer library");
        return;
    }

    auto* create_renderer = reinterpret_cast<rendering::renderer* (*)()>(
        _renderer_lib->resolve("create_renderer"));
    auto* destroy_renderer = reinterpret_cast<void (*)(rendering::renderer*)>(
        _renderer_lib->resolve("destroy_renderer"));
    _renderer =
        std::unique_ptr<rendering::renderer, void (*)(rendering::renderer*)>(
            create_renderer(), destroy_renderer);
    _renderer->initialize(context());

    if (_onInitialized)
    {
        _onInitialized();
    }
}

void Viewport::resizeGL(int w, int h)
{
    auto* f = checkAndGetGLFunctions();
    f->glViewport(0, 0, w, h);
}

void Viewport::paintGL()
{
    common::profile_frame(__FUNCTION__);
    auto* f = checkAndGetGLFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    for (auto& root_object_uuid : _scene->objects())
    {
        auto root_object =
            project::resource_manager::get_resource_static<project::object>(
                root_object_uuid);
        for (auto& object_uuid : root_object->children_uuid())
        {
            auto object =
                project::resource_manager::get_resource_static<project::object>(
                    object_uuid);
            auto rendererComponent =
                object->get_component<project::renderer_component>();
            if (rendererComponent)
            {
                _renderer->render(rendererComponent);
            }
        }
        auto rendererComponent =
            root_object->get_component<project::renderer_component>();
        if (rendererComponent)
        {
            _renderer->render(rendererComponent);
        }
    }

    logger->debug("Viewport painted");
}

QOpenGLFunctions_3_3_Core* Viewport::checkAndGetGLFunctions()
{
    logger->debug("Trying to get OpenGL 3.3 functions");
    QOpenGLFunctions_3_3_Core* f =
        QOpenGLVersionFunctionsFactory::get<QOpenGLFunctions_3_3_Core>(
            context());
    // if (f)
    // {
    //     logger->info("OpenGL 3.3 functions are available");
    //     return f;
    // }

    // f = QOpenGLContext::currentContext()->functions();
    if (!f)
    {
        logger->error("OpenGL functions are not available");
        throw std::runtime_error("OpenGL functions are not available");
    }
    return f;
}

void Viewport::updateShader(std::string_view source,
                            unsigned int& shader,
                            unsigned int shaderType)
{
    auto* f = checkAndGetGLFunctions();

    if (!shader)
    {
        shader = f->glCreateShader(shaderType);
    }

    const char* sourcePtr = source.data();
    f->glShaderSource(shader, 1, &sourcePtr, nullptr);
    f->glCompileShader(shader);

    unsigned int success;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, (int*)&success);
    if (!success)
    {
        std::string infoLog;
        infoLog.resize(512);
        f->glGetShaderInfoLog(shader, 512, nullptr, infoLog.data());
        logger->error(
            "Failed to compile {} shader. Error code {}. Info log: {}",
            shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment",
            success,
            infoLog);
        return;
    }
}

} // namespace g::viewport
