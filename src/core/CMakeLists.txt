add_library(g_core INTERFACE pointer_traits.hpp)
add_library(g::core ALIAS g_core)

target_include_directories(g_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})