#include "project/renderer_component.hpp"

#include "project/assets/mesh_asset.hpp"
#include "project/material.hpp"
#include "project/mesh_component.hpp"
#include "project/resource_manager.hpp"

namespace g::project
{

render_context::~render_context() = default;

renderer_component::renderer_component(std::shared_ptr<class object> parent)
    : component(type(), parent)
{
}

renderer_component::~renderer_component() { }

std::shared_ptr<renderer_component>
renderer_component::create(std::shared_ptr<class object> object)
{
    auto resource =
        std::shared_ptr<renderer_component>(new renderer_component(object));
    resource_manager::get()->register_resource(resource);
    return resource;
}

std::string renderer_component::type() { return "renderer"; }

std::shared_ptr<assets::mesh> renderer_component::mesh() const
{
    return std::static_pointer_cast<assets::mesh>(
        resource_manager::get()->get_resource<asset>(_mesh_uuid));
}

void renderer_component::set_mesh(std::shared_ptr<assets::mesh> mesh)
{
    if (mesh)
    {
        _mesh_uuid = mesh->uuid();
        return;
    }

    _mesh_uuid = {};
}

material* renderer_component::material() const { return _material.get(); }

void renderer_component::set_material(
    std::unique_ptr<class material>&& material)
{
    _material = std::move(material);
}

render_context* renderer_component::render_context() const
{
    return _render_context.get();
}

void renderer_component::set_render_context(
    std::unique_ptr<struct render_context>&& renderContext)
{
    _render_context = std::move(renderContext);
}

} // namespace g::project
