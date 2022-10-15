cmake_minimum_required(VERSION 3.10)

project(gamify)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)

option(QT_VERSION "Set the QT library version" 6)
set(QT_LIB_NAME "Qt${QT_VERSION}")

include_directories(src)
add_subdirectory(src)
