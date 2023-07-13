find_package(${QT_LIB_NAME} REQUIRED COMPONENTS Gui OpenGL)
find_package(Boost REQUIRED)

add_library(g_rendering SHARED renderer.hpp renderer.cpp)
add_library(g::rendering ALIAS g_rendering)

include(GenerateExportHeader)
generate_export_header(g_rendering EXPORT_FILE_NAME g_rendering_export.hpp)

target_include_directories(
  g_rendering PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/
                     ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(g_rendering PRIVATE Qt::Gui Qt::OpenGL Boost::boost
                                          g::common g::project)

# copy to home dir
add_custom_command(
  TARGET g_rendering
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:g_rendering>
          ${CMAKE_SOURCE_DIR}/)
