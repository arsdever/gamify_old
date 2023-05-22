#include <stdafx>

#include "project/scene.hpp"

#include "common/logger.hpp"
#include "project/object.hpp"

namespace g::project
{

namespace
{
common::logger_ptr logger = common::get_logger("scene");
}

scene::scene() = default;

scene_ptr scene::create(std::string_view name)
{
    logger->trace("Creating a scene with name {}", name);
    scene_ptr s { new scene {} };
    s->_name = name;
    return s;
}

} // namespace g::project
