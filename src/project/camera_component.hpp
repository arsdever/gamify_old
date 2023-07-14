#pragma once

#include "project/component.hpp"

namespace g::project
{

class object;

class camera_component : public component
{
private:
    camera_component(std::shared_ptr<class object> parent);

public:
    ~camera_component();

    static std::shared_ptr<camera_component>
    create(std::shared_ptr<class object> parent);
    static std::string type();

private:
};

} // namespace g::project
