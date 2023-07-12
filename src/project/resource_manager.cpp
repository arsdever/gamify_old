#include "project/resource_manager.hpp"

#include "common/logger.hpp"
#include "common/macros.hpp"

namespace g::project
{

using namespace common;

namespace
{
// private logger for debug purposes
common::logger_ptr p_logger = common::get_logger("resources", IS_DEBUG_BUILD);
} // namespace

resource_manager::resource_manager() = default;

resource_manager::~resource_manager() = default;

std::shared_ptr<resource_manager> resource_manager::init()
{
    profile(__FUNCTION__);
    if (_instance)
    {
        p_logger->warn("resource_manager already initialized.");
        return _instance;
    }

    _instance = std::shared_ptr<resource_manager>(new resource_manager());
    return _instance;
}

std::shared_ptr<resource_manager> resource_manager::get()
{
    profile(__FUNCTION__);
    if (!_instance)
    {
        p_logger->warn("resource_manager not initialized.");
        return nullptr;
    }

    return _instance;
}

void resource_manager::deinit()
{
    profile(__FUNCTION__);
    if (!_instance)
    {
        p_logger->warn("resource_manager not initialized.");
        return;
    }

    _instance.reset();
}

void resource_manager::register_resource(std::shared_ptr<resource> resource)
{
    profile(__FUNCTION__);
    if (!resource)
    {
        p_logger->warn("resource is nullptr.");
        return;
    }

    p_logger->trace("Registering resource {}.", resource->uuid());
    _resources[ resource->uuid() ] = resource;
}

void resource_manager::unregister_resource(std::shared_ptr<resource> resource)
{
    profile(__FUNCTION__);
    if (!resource)
    {
        p_logger->warn("resource is nullptr.");
        return;
    }

    p_logger->trace("Unregistering resource {}.", resource->uuid());
    _resources.erase(resource->uuid());
}

void resource_manager::unregister_resource(common::uuid uuid)
{
    profile(__FUNCTION__);
    p_logger->trace("Unregistering resource {}.", uuid);
    _resources.erase(uuid);
}

std::shared_ptr<resource>
resource_manager::get_resource(common::uuid uuid) const
{
    profile(__FUNCTION__);
    auto it = _resources.find(uuid);
    if (it == _resources.end())
    {
        p_logger->warn("Resource {} not found.", uuid);
        return nullptr;
    }

    return it->second;
}

std::shared_ptr<resource_manager> resource_manager::_instance;

} // namespace g::project
