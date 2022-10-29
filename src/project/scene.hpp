#pragma once

#include <project/prototypes.hpp>

#include <boost/signals2.hpp>

namespace g::project
{

class scene
{
public:
    scene(std::string_view name);

    std::string name() const;

    object_ptr create_object(std::string_view name,
                             object_ptr parent = nullptr);

    std::list<object_ptr> objects() const;
    std::list<behavior_ptr> behaviors() const;

#pragma region signals
public:
    boost::signals2::signal<void(object_ptr)> on_object_added;
#pragma endregion

private:
    std::string _name;
    std::list<object_ptr> _objects;
};

} // namespace g::project