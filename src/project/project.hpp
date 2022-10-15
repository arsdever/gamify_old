#pragma once

namespace g::project
{

class project
{
public:
    project(std::string_view name);

private:
    std::string _name;
    scene_ptr _active_scene;
};

} // namespace g::project