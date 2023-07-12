set(HEADERS file_utils.hpp logger.hpp uuid.hpp macros.hpp)
set(SOURCES file_utils.cpp logger.cpp)

if(TARGET prof::profiler)
  list(APPEND SOURCES prof_profiler.cpp)
else()
  list(APPEND HEADERS empty_profiler.cpp)
endif()

add_library(g_common STATIC ${HEADERS} ${SOURCES})
add_library(g::common ALIAS g_common)

find_package(spdlog REQUIRED)
target_link_libraries(g_common PUBLIC spdlog::spdlog)
target_precompile_headers(g_common REUSE_FROM stdafx)
