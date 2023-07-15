#include "project/assets/material_asset.hpp"

#include "project/assets/shader_asset.hpp"
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

std::shared_ptr<shader> material::shader() const
{
    return std::static_pointer_cast<assets::shader>(
        resource_manager::get()->get_resource<asset>(_shader_uuid));
}

const std::unordered_map<std::string, material::property_t>&
material::properties() const
{
    return _properties;
}

material::property_t const& material::property(std::string_view name) const
{
    return _properties.at(name.data());
}

void material::set_shader(std::shared_ptr<class shader> shader)
{
    if (shader)
    {
        _shader_uuid = shader->uuid();
        return;
    }

    _shader_uuid = {};
}

void material::set_property(std::string_view name, property_t value)
{
    _properties[ name.data() ] = value;
}

} // namespace g::project::assets
