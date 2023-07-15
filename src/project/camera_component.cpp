#include "project/camera_component.hpp"

#include "project/resource_manager.hpp"
#include "project/transform_component.hpp"
#include "rendering/gizmo_renderer.hpp"

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

void camera_component::draw_gizmos(rendering::gizmo_renderer& renderer) const
{
    renderer.draw_cube(transform()->position(),
                       common::vector3 { .5, .5, .5 },
                       common::vector3 { 1, 1, 0 });
}

} // namespace g::project
