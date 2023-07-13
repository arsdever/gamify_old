#pragma once

#include "g_rendering_export.hpp"

class QOpenGLContext;

namespace g::project
{
class renderer_component;
class resource_manager;
struct render_context;
} // namespace g::project

namespace g::rendering
{

class G_RENDERING_EXPORT renderer
{
public:
    virtual ~renderer() = 0;

    void setResourceManager(
        std::shared_ptr<project::resource_manager> resourceManager);
    virtual void initialize(QOpenGLContext* context) = 0;
    virtual void
    render(std::shared_ptr<project::renderer_component> renderer) = 0;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) = 0;
};

} // namespace g::rendering

extern "C"
{
    G_RENDERING_EXPORT g::rendering::renderer* create_renderer();
    G_RENDERING_EXPORT void destroy_renderer(g::rendering::renderer* renderer);
}
