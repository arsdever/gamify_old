#pragma once

#include <project/prototypes.hpp>

#include <project/behavior.hpp>
#include <project/behavior_registry.hpp>

namespace g::sample_game
{

class print_behavior : public project::behavior
{
protected:
    print_behavior(project::object_wptr parent);

public:
    static std::shared_ptr<print_behavior> create(project::object_wptr object);

    void set_name(std::string_view name);

    void init() override;
    void update() override;
    void deinit() override;

private:
    std::string _name;
};

} // namespace g::sample_game