#include "project/material.hpp"

namespace g::project
{

material::material() = default;

material::~material() = default;

std::string const& material::vertex_shader_source() const
{
    return _vertex_shader_source;
}

std::string const& material::fragment_shader_source() const
{
    return _fragment_shader_source;
}

void material::set_vertex_shader_source(std::string const& vertexShaderSource)
{
    _vertex_shader_source = vertexShaderSource;
}

void material::set_fragment_shader_source(
    std::string const& fragmentShaderSource)
{
    _fragment_shader_source = fragmentShaderSource;
}

} // namespace g::project
