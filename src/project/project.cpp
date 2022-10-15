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

} // namespace g::project