#include "project/object.hpp"

namespace g::project
{

object::object() = default;

object_ptr object::create(std::string_view name, object_ptr parent)
{
    std::shared_ptr<object> obj { new object() };
    obj->_parent = parent;
    obj->_name = name;

    if (parent)
        parent->add_child(obj);

    return obj;
}

object::~object() = default;

std::string object::name() { return _name; }

std::list<object_ptr> const& object::children() const { return _children; }

object_ptr object::parent() const { return _parent.lock(); }

void object::add_child(object_ptr obj)
{
    obj->_parent = weak_from_this();
    if (std::find(_children.begin(), _children.end(), obj) == _children.end())
        _children.push_back(obj);
}

void object::remove_child(object_ptr obj)
{
    _children.remove(obj);
    obj->_parent.reset();
}

void object::move(object_ptr parent)
{
    if (parent == _parent.lock())
        return;

    if (_parent.lock())
        _parent.lock()->remove_child(shared_from_this());

    _parent = parent;
    parent->add_child(shared_from_this());
}

} // namespace g::project
