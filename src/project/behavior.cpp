#include "project/behavior.hpp"

namespace g::project
{

behavior::behavior(object_wptr object)
    : _object(object)
{
}

void behavior::init() { }

void behavior::update() { }

void behavior::deinit() { }

} // namespace g::project