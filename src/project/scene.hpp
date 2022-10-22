#pragma once

#include <project/prototypes.hpp>

#include <boost/signals2.hpp>
#include <project/object.hpp>

namespace g::project
{

class scene : public object
{
protected:
    scene();

public:
    static scene_ptr create(std::string_view name);
};

} // namespace g::project
