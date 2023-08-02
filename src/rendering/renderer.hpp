#pragma once

#include "common/matrix.hpp"
#include "g_rendering_export.hpp"

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

    virtual void resize(int width, int height);
    void setResourceManager(
        std::shared_ptr<project::resource_manager> resourceManager);
    virtual void initialize() = 0;
    virtual void
    render(std::shared_ptr<project::renderer_component> renderer) = 0;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) = 0;

    void set_projection_matrix(common::matrix4x4f projectionMatrix);
    void set_view_matrix(common::matrix4x4f viewMatrix);

protected:
    int _width;
    int _height;
    common::matrix4x4f _projection_matrix;
    common::matrix4x4f _view_matrix;
};

} // namespace g::rendering

extern "C"
{
    G_RENDERING_EXPORT g::rendering::renderer* create_renderer();
    G_RENDERING_EXPORT void destroy_renderer(g::rendering::renderer* renderer);
}
