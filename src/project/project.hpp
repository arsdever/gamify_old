#pragma once

#include <project/prototypes.hpp>

#include <boost/signals2.hpp>

namespace g::project
{

class project
{
public:
    project(std::string_view name);

    void add_scene(scene_ptr scene);

    void set_name(std::string_view name);
    void set_active_scene(scene_ptr scene);

    std::list<scene_ptr> const& scenes() const;

#pragma region signals
public:
    boost::signals2::signal<void(scene_ptr)> on_scene_added;
    boost::signals2::signal<void(std::string_view)> on_name_changed;
    boost::signals2::signal<void(scene_ptr)> on_active_scene_changed;
#pragma endregion

private:
    std::string _name;
    scene_ptr _active_scene;
    std::list<scene_ptr> _scenes;
};

} // namespace g::project