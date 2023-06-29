set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(${QT_LIB_NAME} CONFIG REQUIRED COMPONENTS Core Gui Widgets)

add_executable(gamify main.cpp resources.qrc)

target_precompile_headers(gamify REUSE_FROM stdafx_qt)
target_link_libraries(gamify Qt::Core Qt::Gui Qt::Widgets g::ui)
