#include "project/component.hpp"

#include "project/object.hpp"
#include "project/resource_manager.hpp"

namespace g::project
{

component::component(std::string_view classname,
                     std::shared_ptr<class object> object)
    : resource(resource_type::component)
    , _classname(classname)
    , _object_uuid(object->uuid())
{
}

component::~component() = default;

std::string const& component::classname() const { return _classname; }

common::uuid component::object_uuid() const { return _object_uuid; }

std::shared_ptr<object> component::object() const
{
    return resource_manager::get()->get_resource<class object>(_object_uuid);
}

bool component::enabled() const { return _enabled; }

void component::set_enabled(bool enabled) { _enabled = enabled; }

void component::enable() { set_enabled(true); }

void component::disable() { set_enabled(false); }

std::shared_ptr<transform_component> component::transform() const
{
    return object()->transform();
}

} // namespace g::project
