find_package(spdlog REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stdafx)

link_libraries(spdlog::spdlog)

add_subdirectory(stdafx)
add_subdirectory(app)
add_subdirectory(core)
add_subdirectory(view)
add_subdirectory(project)
add_subdirectory(engine)
add_subdirectory(sample_game)