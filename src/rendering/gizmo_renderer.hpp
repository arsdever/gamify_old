#pragma once

#include "common/vector.hpp"

namespace g::rendering
{

struct gizmo_renderer
{
    virtual void draw_cube(common::vector3 position,
                           common::vector3 size,
                           common::vector3 color) = 0;
};

} // namespace g::rendering
