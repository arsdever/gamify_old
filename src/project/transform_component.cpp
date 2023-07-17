#include "project/transform_component.hpp"

#include "common/vector.hpp"
#include "project/component.hpp"
#include "project/resource_manager.hpp"

namespace g::project
{

transform_component::transform_component(std::shared_ptr<class object> parent)
    : component(type(), parent)
{
}

transform_component::~transform_component() = default;

std::shared_ptr<transform_component>
transform_component::create(std::shared_ptr<class object> parent)
{
    auto component =
        std::shared_ptr<transform_component>(new transform_component(parent));

    resource_manager::get()->register_resource(component);
    return component;
}

std::string transform_component::type() { return "transform"; }

common::vector3 const& transform_component::position() const
{
    return _position;
}

void transform_component::set_position(common::vector3 const& position)
{
    _position = position;
}

common::quaternion const& transform_component::rotation() const
{
    return _rotation;
}

void transform_component::set_rotation(common::quaternion const& rotation)
{
    _rotation = rotation;
}

common::vector3 const& transform_component::scale() const { return _scale; }

void transform_component::set_scale(common::vector3 const& scale)
{
    _scale = scale;
}

} // namespace g::project
