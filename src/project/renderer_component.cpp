#include "project/renderer_component.hpp"

#include "project/material.hpp"
#include "project/mesh_component.hpp"
#include "project/resource_manager.hpp"

namespace g::project
{

render_context::~render_context() = default;

renderer_component::renderer_component(std::shared_ptr<class object> parent)
    : component("renderer", parent)
{
}

renderer_component::~renderer_component() { }

std::shared_ptr<renderer_component>
renderer_component::create(std::shared_ptr<class object> object)
{
    auto resource = std::shared_ptr<renderer_component>(new renderer_component(object));
    resource_manager::get()->register_resource(resource);
    return resource;
}

std::string renderer_component::type() { return "renderer"; }

material* renderer_component::material() const { return _material.get(); }

void renderer_component::setMaterial(std::unique_ptr<class material>&& material)
{
    _material = std::move(material);
}

render_context* renderer_component::renderContext() const
{
    return _renderContext.get();
}

void renderer_component::setRenderContext(
    std::unique_ptr<render_context>&& renderContext)
{
    _renderContext = std::move(renderContext);
}

} // namespace g::project
