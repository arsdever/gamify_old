#include "project/object.hpp"

#include "common/logger.hpp"
#include "project/component.hpp"
#include "project/transform_component.hpp"
#include "resource_manager.hpp"

namespace g::project
{

namespace
{
common::logger_ptr logger = common::get_logger("object");
}

object::object()
    : resource(resource_type::object)
{
}

object::~object() = default;

std::shared_ptr<object> object::create(std::string_view name,
                                       std::shared_ptr<object> parent)
{
    logger->trace("Creating object {}", name);

    std::shared_ptr<object> object_ { new object() };
    object_->_name = name;

    if (parent)
    {
        object_->_parent_uuid = parent->uuid();
        parent->_children_uuid.push_back(object_->uuid());
    }

    resource_manager::get()->register_resource(object_);

    object_->_transform = transform_component::create(object_);

    return object_;
}

std::string object::name() { return _name; }

std::list<common::uuid> const& object::children_uuid() const
{
    return _children_uuid;
}

common::uuid object::parent_uuid() const { return _parent_uuid; }

std::shared_ptr<object> object::parent() const
{
    if (_parent_uuid == common::uuid {})
        return nullptr;

    return resource_manager::get()->get_resource<object>(parent_uuid());
}

void object::add_child(std::shared_ptr<object> child)
{
    logger->trace("Adding child {} to {}", child->name(), name());
    add_child_uuid(child->uuid());
}

void object::remove_child(std::shared_ptr<object> child)
{
    logger->trace("Removing child {} from {}", child->name(), name());
    remove_child_uuid(child->uuid());
}

void object::change_parent(std::shared_ptr<object> parent)
{
    logger->trace("Setting parent of {} to {}", name(), parent->name());
    change_parent_uuid(parent->uuid());
}

void object::add_child_uuid(common::uuid child_uuid)
{
    logger->trace("Adding child {} to {}", child_uuid, name());
    _children_uuid.push_back(child_uuid);
    resource_manager::get()->get_resource<object>(child_uuid)->_parent_uuid =
        uuid();
}

void object::remove_child_uuid(common::uuid child_uuid)
{
    logger->trace("Removing child {} from {}", child_uuid, name());
    _children_uuid.remove(child_uuid);
    resource_manager::get()->get_resource<object>(child_uuid)->_parent_uuid =
        common::uuid {};
}

void object::change_parent_uuid(common::uuid parent_uuid)
{
    logger->trace("Setting parent of {} to {}", name(), parent_uuid);
    if (_parent_uuid != common::uuid {})
        resource_manager::get()
            ->get_resource<object>(parent_uuid)
            ->_children_uuid.remove(uuid());
    _parent_uuid = parent_uuid;
    if (parent_uuid != common::uuid {})
        resource_manager::get()
            ->get_resource<object>(parent_uuid)
            ->_children_uuid.push_back(uuid());
}

std::shared_ptr<transform_component> object::transform() const
{
    return _transform;
}

void object::add_component(std::shared_ptr<component> component_)
{
    logger->trace("Adding component {} to {}", component_->classname(), name());

    if (std::find(_components_uuid.begin(),
                  _components_uuid.end(),
                  component_->uuid()) != _components_uuid.end())
    {
        logger->warn("Component {} already added to {}",
                     component_->classname(),
                     name());
        return;
    }

    _components_uuid.push_back(component_->uuid());
}

std::shared_ptr<component> object::get_component(std::string_view classname)
{
    for (auto const& component_uuid : _components_uuid)
    {
        std::shared_ptr<component> c =
            resource_manager::get()->get_resource<component>(component_uuid);
        if (c->classname() == classname)
            return c;
    }
    return nullptr;
}

} // namespace g::project
