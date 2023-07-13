find_package(Boost REQUIRED)
find_package(assimp REQUIRED)

set(HEADERS
    asset.hpp
    asset_manager.hpp
    component.hpp
    material.hpp
    mesh_component.hpp
    object.hpp
    project.hpp
    renderer_component.hpp
    resource.hpp
    resource_manager.hpp
    scene.hpp
    assets/mesh_asset.hpp)
set(SOURCES
    asset.cpp
    asset_manager.cpp
    component.cpp
    material.cpp
    mesh_component.cpp
    object.cpp
    project.cpp
    renderer_component.cpp
    resource.cpp
    resource_manager.cpp
    scene.cpp
    assets/mesh_asset.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_link_libraries(g_project PUBLIC assimp::assimp Boost::boost g::common)
target_precompile_headers(g_project REUSE_FROM stdafx)
