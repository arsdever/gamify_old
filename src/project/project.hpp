#pragma once

#include <boost/signals2.hpp>

namespace g::project
{

class project
{
public:
    project(std::string_view name);

    void set_name(std::string_view name);
    void set_active_scene(scene_ptr scene);

#pragma region signals
public:
    boost::signals2::signal<void(std::string_view)> signal_name_changed;
    boost::signals2::signal<void(scene_ptr)> signal_active_scene_changed;

private:
    std::string _name;
    scene_ptr _active_scene;
};

} // namespace g::project