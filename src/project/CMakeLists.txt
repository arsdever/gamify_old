find_package(Boost REQUIRED)

set(HEADERS behavior.hpp game_object.hpp project.hpp object.hpp scene.hpp prototypes.hpp)
set(SOURCES behavior.cpp game_object.cpp project.cpp object.cpp scene.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_link_libraries(g_project PUBLIC Boost::boost g::core)
target_precompile_headers(g_project REUSE_FROM stdafx)
