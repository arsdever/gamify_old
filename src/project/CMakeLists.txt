find_package(Boost REQUIRED)
find_package(assimp REQUIRED)

set(HEADERS
    asset.hpp
    asset_manager.hpp
    camera_component.hpp
    component.hpp
    object.hpp
    project.hpp
    renderer_component.hpp
    resource.hpp
    resource_manager.hpp
    scene.hpp
    transform_component.hpp
    assets/mesh_asset.hpp
    assets/material_asset.hpp
    assets/shader_asset.hpp)
set(SOURCES
    asset.cpp
    asset_manager.cpp
    camera_component.cpp
    component.cpp
    object.cpp
    project.cpp
    renderer_component.cpp
    resource.cpp
    resource_manager.cpp
    scene.cpp
    transform_component.cpp
    assets/mesh_asset.cpp
    assets/material_asset.cpp
    assets/shader_asset.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_link_libraries(g_project PUBLIC assimp::assimp Boost::boost g::common)
target_precompile_headers(g_project REUSE_FROM stdafx)
