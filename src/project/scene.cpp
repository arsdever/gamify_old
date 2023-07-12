#include "project/scene.hpp"

#include "common/logger.hpp"
#include "common/profiler.hpp"
#include "fmt/format.h"
#include "project/object.hpp"
#include "resource_manager.hpp"

namespace g::project
{

namespace
{
common::logger_ptr logger = common::get_logger("scene");
}

scene::scene()
    : resource(resource_type::scene)
{
}

scene::~scene() = default;

std::shared_ptr<scene> scene::create(std::string_view name)
{
    logger->trace("Creating scene {}", name);

    std::shared_ptr<scene> scene_ { new scene() };
    scene_->_name = name;

    return scene_;
}

std::string scene::name() const { return _name; }

std::list<common::uuid> const& scene::objects() const { return _objects_uuid; }

void scene::add_object(std::shared_ptr<object> object)
{
    common::profile(__FUNCTION__);
    logger->trace("Adding object {} to {}", object->name(), name());

    _objects_uuid.push_back(object->uuid());
}

void scene::remove_object(std::shared_ptr<object> object)
{
    common::profile(__FUNCTION__);
    logger->trace("Removing object {} from {}", object->name(), name());

    _objects_uuid.remove(object->uuid());
}

} // namespace g::project
