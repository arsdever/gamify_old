#pragma once

#include <string>
#include <unordered_map>
#include <variant>

#include "common/color.hpp"
#include "project/asset.hpp"

namespace g::project::assets
{

class shader;

class material : public asset
{
public:
    using property_t = std::
        variant<double, float, int, unsigned, bool, std::string, common::color>;

private:
    material(std::string_view name);

public:
    ~material();

    static std::shared_ptr<material> create(std::string_view name);

    std::shared_ptr<class shader> shader() const;
    const std::unordered_map<std::string, property_t>& properties() const;
    property_t const& property(std::string_view name) const;

    void set_shader(std::shared_ptr<class shader> shader);
    void set_property(std::string_view name, property_t value);

private:
    common::uuid _shader_uuid;
    std::unordered_map<std::string, property_t> _properties;
};

} // namespace g::project::assets
