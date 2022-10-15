#pragma once

namespace g::project
{

class scene
{
public:
    scene(std::string_view name);

private:
    std::string _name;
};

} // namespace g::project