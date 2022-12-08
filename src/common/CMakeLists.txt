set(HEADERS file_utils.hpp logger.hpp)
set(SOURCES file_utils.cpp logger.cpp)

add_library(g_common STATIC ${HEADERS} ${SOURCES})
add_library(g::common ALIAS g_common)

find_package(spdlog REQUIRED)
target_link_libraries(g_common PUBLIC spdlog::spdlog)
target_precompile_headers(g_common REUSE_FROM stdafx)