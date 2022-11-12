#include <stdafx>

#include "project/scene.hpp"

#include "project/object.hpp"

namespace g::project
{

scene::scene() = default;

scene_ptr scene::create(std::string_view name)
{
    scene_ptr s { new scene {} };
    s->_name = name;
    return s;
}

} // namespace g::project
