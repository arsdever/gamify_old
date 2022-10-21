#include <stdafx>

#include "project/scene.hpp"

#include "project/object.hpp"

namespace g::project
{

scene::scene(std::string_view name)
    : _name(name)
{
}

std::string scene::name() const { return _name; }

object_ptr scene::create_object(std::string_view name, object_ptr parent)
{
    object_ptr obj = std::make_shared<object>(name, parent);
    _objects.push_back(obj);
    on_object_added(obj);
    return obj;
}

void scene::remove_object(object_ptr obj)
{
    _objects.erase(std::find(_objects.begin(), _objects.end(), obj));
    on_object_removed(obj);
}

std::list<object_ptr> const& scene::objects() const { return _objects; }

} // namespace g::project
