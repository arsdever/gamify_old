#include "project/object.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(object, creation)
{
    std::shared_ptr<g::project::object> object =
        g::project::object::create("Object 1", nullptr);

    EXPECT_EQ(object->name(), "Object 1");
    EXPECT_EQ(object->parent(), nullptr);
}

TEST(object, relation_add)
{
    std::shared_ptr<g::project::object> parent =
        g::project::object::create("Parent object", nullptr);
    std::shared_ptr<g::project::object> child =
        g::project::object::create("Child object", nullptr);

    EXPECT_EQ(child->parent(), nullptr);
    EXPECT_EQ(parent->children_uuid().size(), 0);

    parent->add_child(child);

    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(parent->children_uuid().size(), 1);
}

TEST(object, relation_on_creation)
{
    std::shared_ptr<g::project::object> parent =
        g::project::object::create("Parent object", nullptr);
    std::shared_ptr<g::project::object> child2 =
        g::project::object::create("Child object 2", parent);

    EXPECT_EQ(child2->parent(), parent);
    EXPECT_EQ(parent->children_uuid().size(), 1);
}

TEST(object, remove)
{
    std::shared_ptr<g::project::object> parent =
        g::project::object::create("Parent object", nullptr);
    std::shared_ptr<g::project::object> child =
        g::project::object::create("Child object", parent);

    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(parent->children_uuid().size(), 1);

    parent->remove_child(child);

    EXPECT_EQ(child->parent(), nullptr);
    EXPECT_EQ(parent->children_uuid().size(), 0);
}
