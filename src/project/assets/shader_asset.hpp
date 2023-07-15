#pragma once

#include <string>

#include "project/asset.hpp"

namespace g::project::assets
{

class shader : public asset
{
private:
    shader(std::string_view name);

public:
    ~shader();

    static std::shared_ptr<shader> create(std::string_view name);

    const std::string& vertex_shader_source() const;
    const std::string& fragment_shader_source() const;

    void set_vertex_shader_source(std::string_view vertex_shader_source);
    void set_fragment_shader_source(std::string_view fragment_shader_source);

private:
    std::string _vertex_shader_source;
    std::string _fragment_shader_source;
};

} // namespace g::project::assets
