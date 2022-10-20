#include <stdafx>

#include "project/project.hpp"

#include "project/scene.hpp"

namespace g::project
{

project::project(std::string_view name)
    : _name(name)
    , _active_scene(nullptr)
{
}

void project::add_scene(scene_ptr scene)
{
    _scenes.push_back(scene);
    on_scene_added(scene);
}

void project::set_name(std::string_view name)
{
    _name = name;
    on_name_changed(name);
}

void project::set_active_scene(scene_ptr scene)
{
    _active_scene = scene;
    on_active_scene_changed(scene);
}

std::list<scene_ptr> const& project::scenes() const { return _scenes; }

} // namespace g::project