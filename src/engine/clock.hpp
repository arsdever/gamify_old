#pragma once

#include <engine/prototypes.hpp>

namespace g::engine
{

class clock
{
private:
    clock();

public:
    static clock_ptr create();
    void update();
    float get_delta_time() const;
    float get_time() const;
};

}; // namespace g::engine
