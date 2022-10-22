# https://google.github.io/googletest/quickstart-cmake.html#set-up-a-project
# Download GTest package
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(unittest main.cpp object.cpp)
target_link_libraries(unittest gtest gmock stdafx g::project)
target_precompile_headers(unittest REUSE_FROM stdafx)
gtest_discover_tests(unittest)
