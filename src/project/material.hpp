#pragma once

#include <string>

namespace g::project
{

class material
{
public:
    material();
    ~material();

    std::string const& vertex_shader_source() const;
    std::string const& fragment_shader_source() const;

    void set_vertex_shader_source(std::string const& vertexShaderSource);
    void set_fragment_shader_source(std::string const& fragmentShaderSource);

private:
    std::string _vertex_shader_source;
    std::string _fragment_shader_source;
};

} // namespace g::project
