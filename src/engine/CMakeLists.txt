set(HEADERS engine.hpp)
set(SOURCES engine.cpp)

add_library(g_engine STATIC ${HEADERS} ${SOURCES})
add_library(g::engine ALIAS g_engine)

target_link_libraries(g_engine PUBLIC g::project)
target_precompile_headers(g_engine REUSE_FROM stdafx)
