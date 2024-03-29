set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(${QT_LIB_NAME} REQUIRED COMPONENTS Core Widgets)
find_package(Boost REQUIRED)
find_package(assimp REQUIRED)

set(HEADERS main_window.hpp scene_model.hpp scene_view.hpp properties_panel.hpp)
set(SOURCES main_window.cpp scene_model.cpp scene_view.cpp properties_panel.cpp)

add_library(g_ui STATIC ${HEADERS} ${SOURCES})
add_library(g::ui ALIAS g_ui)

target_link_libraries(
  g_ui
  PUBLIC Qt::Core
         Qt::Widgets
         assimp::assimp
         Boost::boost
         g::project
         g::viewport
         qspdlog::lib)
