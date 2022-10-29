#pragma once

#include <spdlog>

#include <project/behavior.hpp>

namespace g::sample_game
{

print_behavior::print_behavior(std::string_view name)
    : _name { name }
{
}

auto print_behavior::create(std::string_view name)
    -> std::shared_ptr<print_behavior>
{
    return std::make_shared<print_behavior>(name);
}

void print_behavior::init() { spdlog::info("Hello from {}!", _name); }

void print_behavior::update() { spdlog::info("{} is still alive!", _name); }

void print_behavior::deinit() { spdlog::info("Goodbye from {}!", _name); }

project::behavior_registry::register_type<print_behavior>("print_behavior");

} // namespace g::sample_game