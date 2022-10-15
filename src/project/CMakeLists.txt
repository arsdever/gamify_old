set(HEADERS project.hpp)
set(SOURCES project.cpp)

add_library(g_project STATIC ${SOURCES} ${HEADERS})
add_library(g::project ALIAS g_project)

target_precompile_headers(g_project REUSE_FROM stdafx)
