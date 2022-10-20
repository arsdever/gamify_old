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

} // namespace g::project
