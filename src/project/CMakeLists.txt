set(HEADERS project.hpp scene.hpp)
set(SOURCES project.cpp scene.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_precompile_headers(g_project REUSE_FROM stdafx)
