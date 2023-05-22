find_package(Boost REQUIRED)

set(HEADERS project.hpp object.hpp scene.hpp prototypes.hpp)
set(SOURCES project.cpp object.cpp scene.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_link_libraries(g_project PUBLIC Boost::boost g::common)
target_precompile_headers(g_project REUSE_FROM stdafx)
