#include <stdafx>

#include "project/scene.hpp"

namespace g::project
{

scene::scene(std::string_view name)
    : _name(name)
{
}

std::string scene::name() const { return _name; }

} // namespace g::project