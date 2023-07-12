#pragma once

#include <boost/signals2.hpp>

namespace g::project
{

class scene;

class project
{
public:
    project(std::string_view name);

    void add_scene(std::shared_ptr<scene> scene);

    void set_name(std::string_view name);
    void set_active_scene(std::shared_ptr<scene> scene);

    std::shared_ptr<scene> active_scene() const;
    std::list<std::shared_ptr<scene>> const& scenes() const;

#pragma region signals
public:
    boost::signals2::signal<void(std::shared_ptr<scene>)> on_scene_added;
    boost::signals2::signal<void(std::string_view)> on_name_changed;
    boost::signals2::signal<void(std::shared_ptr<scene>)>
        on_active_scene_changed;
#pragma endregion

private:
    std::string _name;
    std::shared_ptr<scene> _active_scene;
    std::list<std::shared_ptr<scene>> _scenes;
};

} // namespace g::project
