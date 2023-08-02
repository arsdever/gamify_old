#include <QEvent>
#include <QLibrary>
#include <QOpenGLContext>
#include <QResizeEvent>

#include "viewport/viewport.hpp"

#include "common/logger.hpp"
#include "common/profiler.hpp"
#include "project/camera_component.hpp"
#include "project/renderer_component.hpp"
#include "project/resource_manager.hpp"
#include "project/scene.hpp"
#include "project/transform_component.hpp"
#include "rendering/renderer.hpp"

namespace g::viewport
{
namespace
{
common::logger_ptr logger = common::get_logger("viewport");
}

Viewport::Viewport()
    : _renderer_lib(nullptr)
    , _renderer(nullptr, nullptr)
{
    setSurfaceType(QSurface::OpenGLSurface);
    _renderer->setResourceManager(project::resource_manager::get());
}

Viewport::~Viewport() = default;

void Viewport::setCamera(std::shared_ptr<project::camera_component> camera)
{
    _camera = camera;

    if (height())
        _projection = common::matrix4x4f::perspective(
            45.0f,
            static_cast<float>(width()) / static_cast<float>(height()),
            0.1f,
            100.0f);

    recalculateViewMatrix();
    renderLater();
}

std::shared_ptr<project::camera_component> Viewport::camera() const
{
    return _camera;
}

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

void Viewport::initialize()
{
    common::profile(__FUNCTION__);
    QSurfaceFormat format;
    format.setVersion(3, 3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setSamples(16);
    setFormat(format);

    if (!_context)
    {
        _context = new QOpenGLContext(this);
        _context->setFormat(requestedFormat());
        _context->create();
    }
    _context->makeCurrent(this);

    if (!_needsInitialize)
    {
        return;
    }

    _needsInitialize = false;
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
    _renderer->initialize();

    if (_onInitialized)
    {
        _onInitialized();
    }
}

bool Viewport::event(QEvent* event)
{
    switch (event->type())
    {
    case QEvent::Expose: initialize();
    case QEvent::UpdateRequest:
    {
        if (isExposed())
            render();

        return true;
    }
    case QEvent::Resize:
    {
        if (!_renderer)
        {
            return false;
        }

        common::profile(__FUNCTION__);
        QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
        _renderer->resize(resizeEvent->size().width(),
                          resizeEvent->size().height());
        _projection = common::matrix4x4f::perspective(
            45.0f,
            static_cast<float>(width()) / static_cast<float>(height()),
            0.1f,
            100.0f);
        renderLater();
        break;
    }
    default: break;
    }

    return QWindow::event(event);
}

void Viewport::renderLater() { requestUpdate(); }

void Viewport::render()
{
    common::profile(__FUNCTION__);

    _context->makeCurrent(this);

    _renderer->set_projection_matrix(_projection);
    _renderer->set_view_matrix(_view);

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

    _context->swapBuffers(this);

    logger->debug("Viewport painted");
    renderLater();
}

void Viewport::recalculateViewMatrix()
{
    auto position = _camera->object()->transform()->position();
    _view = common::matrix4x4f::from_look(position, { 0, 0, 0 }, { 0, 1, 0 });
}

} // namespace g::viewport
