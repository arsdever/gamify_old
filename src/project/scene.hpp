#pragma once

#include <project/prototypes.hpp>

namespace g::project
{

class scene
{
public:
    scene(std::string_view name);

    std::string name() const;

private:
    std::string _name;
};

} // namespace g::project