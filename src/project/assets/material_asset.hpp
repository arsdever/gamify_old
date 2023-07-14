#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "common/color.hpp"
#include "project/asset.hpp"

namespace g::project::assets
{

class material : public asset
{
public:
    using property_t = std::
        variant<double, float, int, unsigned, bool, std::string, common::color>;

public:
    material(std::string_view name);
    ~material();

    static std::shared_ptr<material> create(std::string_view name);

    std::string const& vertex_shader_source() const;
    std::string const& fragment_shader_source() const;
    const std::unordered_map<std::string, property_t>& properties() const;
    property_t const& property(std::string_view name) const;

    void set_vertex_shader_source(std::string_view vertex_shader_source);
    void set_fragment_shader_source(std::string_view fragment_shader_source);
    void set_property(std::string_view name, property_t value);

private:
    std::string _vertex_shader_source;
    std::string _fragment_shader_source;
    std::unordered_map<std::string, property_t> _properties;
};

} // namespace g::project::assets
