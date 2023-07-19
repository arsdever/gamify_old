#include "project/project.hpp"

#include "common/logger.hpp"
#include "project/scene.hpp"

namespace g::project
{

namespace
{
common::logger_ptr logger = common::get_logger("project");
}

project::project(std::string_view name)
    : _name(name)
    , _active_scene(nullptr)
{
    logger->trace("Project created: {}", name);
}

void project::add_scene(std::shared_ptr<scene> scene)
{
    _scenes.push_back(scene);
    logger->trace("Scene added: {}", scene->name());
    on_scene_added(scene);
}

void project::set_name(std::string_view name)
{
    _name = name;
    logger->trace("Project name changed: {}", name);
    on_name_changed(name);
}

void project::set_active_scene(std::shared_ptr<scene> scene)
{
    _active_scene = scene;
    logger->trace("Active scene changed: {}", scene->name());
    on_active_scene_changed(scene);
}

std::shared_ptr<scene> project::active_scene() const { return _active_scene; }

std::list<std::shared_ptr<scene>> const& project::scenes() const
{
    return _scenes;
}

} // namespace g::project
