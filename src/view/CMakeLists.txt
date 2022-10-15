set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(${QT_LIB_NAME} REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED)

set(HEADERS main_window.hpp project_widget.hpp)
set(SOURCES main_window.cpp project_widget.cpp)

add_library(g_view STATIC ${HEADERS} ${SOURCES})
add_library(g::view ALIAS g_view)

target_link_libraries(g_view PUBLIC Qt::Widgets Boost::boost g::project)
target_precompile_headers(g_view REUSE_FROM stdafx_qt)
