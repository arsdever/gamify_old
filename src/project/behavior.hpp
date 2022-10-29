#pragma once

#include <project/prototypes.hpp>

#include <project/object.hpp>

namespace g::project
{

class behavior : public object
{
public:
    behavior(object_wptr object);
    virtual ~behavior() = default;

    virtual void init();
    virtual void update();
    virtual void deinit();

private:
    object_wptr _object;
};

} // namespace g::project