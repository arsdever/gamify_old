find_package(Boost REQUIRED)
find_package(assimp REQUIRED)

set(HEADERS
    component.hpp
    object.hpp
    project.hpp
    prototypes.hpp
    renderer_component.hpp
    resource.hpp
    resource_manager.hpp
    scene.hpp)
set(SOURCES
    component.cpp
    object.cpp
    project.cpp
    renderer_component.cpp
    resource.cpp
    resource_manager.cpp
    scene.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_link_libraries(g_project PUBLIC assimp::assimp Boost::boost g::common)
target_precompile_headers(g_project REUSE_FROM stdafx)
