cmake_minimum_required(VERSION 3.10)

project(gamify)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_CXX_STANDARD 20)

option(QT_VERSION "Set the QT library version" 6)
set(QT_LIB_NAME "Qt${QT_VERSION}")

option(GAMIFY_BUILD_TESTS "Build tests for gamify project" ON)

include_directories(src)
add_subdirectory(src)

if(GAMIFY_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
