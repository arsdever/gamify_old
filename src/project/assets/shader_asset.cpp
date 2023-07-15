#include "project/assets/shader_asset.hpp"

#include "project/resource_manager.hpp"

namespace g::project::assets
{

shader::shader(std::string_view name)
    : asset(name, asset_type::shader)
{
}

shader::~shader() = default;

std::shared_ptr<shader> shader::create(std::string_view name)
{
    auto resource = std::shared_ptr<shader>(new shader(name));

    resource_manager::get()->register_resource(resource);
    return resource;
}

const std::string& shader::vertex_shader_source() const
{
    return _vertex_shader_source;
}

const std::string& shader::fragment_shader_source() const
{
    return _fragment_shader_source;
}

void shader::set_vertex_shader_source(std::string_view vertex_shader_source)
{
    _vertex_shader_source = vertex_shader_source;
}

void shader::set_fragment_shader_source(std::string_view fragment_shader_source)
{
    _fragment_shader_source = fragment_shader_source;
}

} // namespace g::project::assets
