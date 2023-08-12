#include "project/transform_component.hpp"

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

common::vector3f const& transform_component::position() const
{
    return _position;
}

void transform_component::set_position(common::vector3f const& position)
{
    if (_position != position)
    {
        _is_dirty = true;
        _position = position;
    }
}

common::quaternion const& transform_component::rotation() const
{
    return _rotation;
}

void transform_component::set_rotation(common::quaternion const& rotation)
{
    if (_rotation != rotation)
    {
        _is_dirty = true;
        _rotation = rotation;
    }
}

common::vector3f const& transform_component::scale() const { return _scale; }

void transform_component::set_scale(common::vector3f const& scale)
{
    if (_scale != scale)
    {
        _is_dirty = true;
        _scale = scale;
    }
}

void transform_component::look_at(common::vector3f const& target)
{
    _rotation = common::quaternion::look_rotation(_position, target);
}

common::matrix4x4f const& transform_component::matrix() const
{
    if (_is_dirty)
    {
        update_matrix();
        _is_dirty = false;
    }
    return _matrix;
}

void transform_component::update_matrix() const
{
    auto rotmat = _rotation.to_matrix();
    _matrix[ 0 ] = { _scale.x() * rotmat[ 0 ], rotmat[ 3 ], rotmat[ 6 ], 0.0f };
    _matrix[ 1 ] = { rotmat[ 1 ], _scale.y() * rotmat[ 4 ], rotmat[ 7 ], 0.0f };
    _matrix[ 2 ] = { rotmat[ 2 ], rotmat[ 5 ], _scale.z() * rotmat[ 8 ], 0.0f };
    _matrix[ 3 ] = { _position.x(), _position.y(), _position.z(), 1.0f };
}

} // namespace g::project
