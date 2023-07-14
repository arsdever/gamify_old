#pragma once

#include "common/matrix.hpp"
#include "g_rendering_export.hpp"

namespace g::common
{
struct matrix4x4;
} // namespace g::common

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
    virtual void initialize() = 0;
    virtual void
    render(std::shared_ptr<project::renderer_component> renderer) = 0;
    virtual void
    load_object(std::shared_ptr<project::renderer_component> renderer) = 0;

    void set_projection_matrix(common::matrix4x4 projectionMatrix);
    void set_view_matrix(common::matrix4x4 viewMatrix);

protected:
    common::matrix4x4 _projection_matrix;
    common::matrix4x4 _view_matrix;
};

} // namespace g::rendering

extern "C"
{
    G_RENDERING_EXPORT g::rendering::renderer* create_renderer();
    G_RENDERING_EXPORT void destroy_renderer(g::rendering::renderer* renderer);
}
