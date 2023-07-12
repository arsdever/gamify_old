#include "project/resource_manager.hpp"
#include <gtest/gtest.h>

int main(int argc, char** argv)
{
    g::project::resource_manager::init();
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    g::project::resource_manager::deinit();
    return result;
}
