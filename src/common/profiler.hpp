#pragma once

#include <string_view>

namespace g::common
{

void profile(std::string_view function_name);

void profile_frame(std::string_view frame_name);

} // namespace g::common
