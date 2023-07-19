#pragma once

#include "common/vector.hpp"
#include "common/quaternion.hpp"
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

private:
	common::vector3f _position;
	common::quaternion _rotation;
	common::vector3f _scale;
};

} // namespace g::project
