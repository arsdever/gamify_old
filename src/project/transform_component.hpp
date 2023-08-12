#pragma once

#include "common/matrix.hpp"
#include "common/quaternion.hpp"
#include "common/vector.hpp"
#include "project/component.hpp"

namespace g::project
{

class object;

class transform_component : public component
{
private:
    transform_component(std::shared_ptr<class object> parent);

public:
    ~transform_component();

    static std::shared_ptr<transform_component>
    create(std::shared_ptr<class object> parent);
    static std::string type();

    common::vector3f const& position() const;
    void set_position(common::vector3f const& position);

    common::quaternion const& rotation() const;
    void set_rotation(common::quaternion const& rotation);

    common::vector3f const& scale() const;
    void set_scale(common::vector3f const& scale);

    void look_at(common::vector3f const& target);

    common::matrix4x4f const& matrix() const;

private:
    void update_matrix() const;

private:
    common::vector3f _position { 0, 0, 0 };
    common::quaternion _rotation { 0, 0, 0, 1 };
    common::vector3f _scale { 1, 1, 1 };
    mutable common::matrix4x4f _matrix { common::matrix4x4f::identity() };
    mutable bool _is_dirty = true;
};

} // namespace g::project
