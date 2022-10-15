find_package(${QT_LIB_NAME} CONFIG REQUIRED COMPONENTS Core Gui Widgets)

add_library(stdafx SHARED stdafx.cpp stdafx)
add_library(stdafx_qt SHARED stdafx_qt.cpp stdafx stdafx_qt)

target_precompile_headers(stdafx PUBLIC stdafx)
target_precompile_headers(stdafx_qt PUBLIC stdafx stdafx_qt)

target_link_libraries(stdafx_qt Qt::Core Qt::Gui Qt::Widgets)
