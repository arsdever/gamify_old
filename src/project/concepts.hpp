#pragma once

#include <type_traits>

namespace g::project
{

class resource;
template <typename T>
concept is_resource = std::is_base_of_v<resource, T>;

class component;
template <typename T>
concept is_component = std::is_base_of_v<component, T>;

} // namespace g::project
