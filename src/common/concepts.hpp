#pragma once

#include <type_traits>

namespace g::common
{

template <typename T>
concept is_scalar = std::is_scalar_v<T>;

} // namespace g::common
