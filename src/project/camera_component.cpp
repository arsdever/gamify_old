#include "project/camera_component.hpp"

#include "project/resource_manager.hpp"

namespace g::project
{

camera_component::camera_component(std::shared_ptr<class object> parent)
    : component(type(), parent)
{
}

camera_component::~camera_component() = default;

std::shared_ptr<camera_component>
camera_component::create(std::shared_ptr<class object> parent)
{
    auto component =
        std::shared_ptr<camera_component>(new camera_component(parent));

    resource_manager::get()->register_resource(component);
    return component;
}

std::string camera_component::type() { return "camera"; }

} // namespace g::project
