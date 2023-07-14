#pragma once

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

	common::vector3 const& position() const;
	void set_position(common::vector3 const& position);

	common::vector3 const& rotation() const;
	void set_rotation(common::vector3 const& rotation);

	common::vector3 const& scale() const;
	void set_scale(common::vector3 const& scale);

private:
	common::vector3 _position;
	common::vector3 _rotation;
	common::vector3 _scale;
};

} // namespace g::project
