#include "project/behavior.hpp"

namespace g::project
{

behavior::behavior(object_wptr object)
    : _object(object)
{
}

behavior_ptr behavior::create(object_wptr object)
{
    return std::shared_ptr<behavior>(new behavior { object });
}

void behavior::init() { }

void behavior::update() { }

void behavior::deinit() { }

} // namespace g::project