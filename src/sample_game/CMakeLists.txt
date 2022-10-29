set(HEADERS print_behavior.hpp)
set(SOURCES print_behavior.cpp)

add_library(sample_game SHARED ${HEADERS} ${SOURCES})

target_link_libraries(sample_game PRIVATE g::project)