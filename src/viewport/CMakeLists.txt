set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(${QT_LIB_NAME} REQUIRED COMPONENTS OpenGLWidgets)
find_package(Boost REQUIRED)

set(HEADERS camera_view.hpp viewport.hpp)
set(SOURCES camera_view.cpp viewport.cpp)

add_library(g_viewport STATIC ${HEADERS} ${SOURCES})
add_library(g::viewport ALIAS g_viewport)

target_link_libraries(g_viewport PUBLIC Qt::OpenGLWidgets g::common g::project
                                        g::rendering)
