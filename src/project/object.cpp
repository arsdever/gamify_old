#include "project/object.hpp"

namespace g::project
{

object::object(std::string_view name, object_ptr parent)
    : _parent { parent }
    , _name { name }
{
}

object::~object() = default;

std::string object::name() { return _name; }

std::list<object_ptr> const& object::children() const { return _children; }

object_ptr object::parent() const { return _parent.lock(); }

void object::add_child(object_ptr child)
{
    _children.push_back(child);
    on_children_list_changed();
}

void object::remove_child(object_ptr child)
{
    _children.erase(std::find(_children.begin(), _children.end(), child));
    on_children_list_changed();
}

} // namespace g::project
