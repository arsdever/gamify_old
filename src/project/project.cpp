#include <stdafx>

#include "project/project.hpp"

#include "project/scene.hpp"

namespace g::project
{

project::project(std::string_view name)
    : _name(name)
    , _active_scene(std::make_shared<scene>("Untitled"))
{
}

void project::set_name(std::string_view name)
{
    _name = name;
    signal_name_changed(name);
}

void project::set_active_scene(scene_ptr scene)
{
    _active_scene = scene;
    signal_active_scene_changed(scene);
}

} // namespace g::project