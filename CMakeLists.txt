cmake_minimum_required(VERSION 3.10)

project(gamify)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

set(CMAKE_CXX_STANDARD 20)
include($ENV{VCPKG}/scripts/buildsystems/vcpkg.cmake)

add_subdirectory(src)
