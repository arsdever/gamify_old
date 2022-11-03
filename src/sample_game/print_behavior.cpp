#include <stdafx>

#include "print_behavior.hpp"

#include <sample_game/logger.hpp>

namespace g::sample_game
{

print_behavior::print_behavior(project::object_wptr parent)
    : behavior(parent)
{
}

std::shared_ptr<print_behavior>
print_behavior::create(project::object_wptr object)
{
    return std::shared_ptr<print_behavior>(new print_behavior { object });
}

void print_behavior::set_name(std::string_view name) { _name = name; }

void print_behavior::init() { spdlog::info("Hello from {}!", _name); }

void print_behavior::update() { spdlog::info("{} is still alive!", _name); }

void print_behavior::deinit() { spdlog::info("Goodbye from {}!", _name); }

} // namespace g::sample_game

extern "C" __declspec(dllexport) void register_behavior_types(
    g::project::behavior_registry* registry)
{
    g::sample_game::logger()->info("Registering behavior types...");
    registry->register_type<g::sample_game::print_behavior>("print");
}

extern "C" __declspec(dllexport) void unregister_behavior_types(
    g::project::behavior_registry* registry)
{
    g::sample_game::logger()->info("Unregistering behavior types...");
    registry->unregister_type("print");
}