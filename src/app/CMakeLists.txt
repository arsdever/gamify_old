find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Gui Widgets)

add_executable(gamify main.cpp)

target_precompile_headers(gamify REUSE_FROM stdafx_qt)
target_link_libraries(gamify Qt::Core Qt::Gui Qt::Widgets g::view)
