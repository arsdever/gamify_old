#pragma once

#include "project/asset.hpp"

namespace g::project::assets
{

class material : public asset
{
public:
    material(std::string_view name);
    ~material();

    static std::shared_ptr<material> create(std::string_view name);

    std::string const& vertex_shader_source() const;
    std::string const& fragment_shader_source() const;

    void set_vertex_shader_source(std::string_view vertex_shader_source);
    void set_fragment_shader_source(std::string_view fragment_shader_source);

private:
    std::string _vertex_shader_source;
    std::string _fragment_shader_source;
};

} // namespace g::project::assets
