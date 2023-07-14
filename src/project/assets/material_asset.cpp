#include "project/assets/material_asset.hpp"

#include "project/resource_manager.hpp"

namespace g::project::assets
{

material::material(std::string_view name)
    : asset(name, asset_type::material)
{
}

material::~material() = default;

std::shared_ptr<material> material::create(std::string_view name)
{
    auto resource = std::shared_ptr<material>(new material(name));

    resource_manager::get()->register_resource(resource);
    return resource;
}

std::string const& material::vertex_shader_source() const
{
    return _vertex_shader_source;
}

std::string const& material::fragment_shader_source() const
{
    return _fragment_shader_source;
}

void material::set_vertex_shader_source(std::string_view vertex_shader_source)
{
    _vertex_shader_source = vertex_shader_source;
}

void material::set_fragment_shader_source(
    std::string_view fragment_shader_source)
{
    _fragment_shader_source = fragment_shader_source;
}

} // namespace g::project::assets
