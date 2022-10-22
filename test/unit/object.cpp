#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <project/object.hpp>

TEST(object, creation)
{
    g::project::object_ptr object =
        g::project::object::create("Object 1", nullptr);

    EXPECT_EQ(object->name(), "Object 1");
    EXPECT_EQ(object->parent(), nullptr);
}

TEST(object, relation_add)
{
    g::project::object_ptr parent =
        g::project::object::create("Parent object", nullptr);
    g::project::object_ptr child =
        g::project::object::create("Child object", nullptr);

    EXPECT_EQ(child->parent(), nullptr);
    EXPECT_EQ(parent->children().size(), 0);

    parent->add_child(child);

    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(parent->children().size(), 1);
}

TEST(object, relation_on_creation)
{
    g::project::object_ptr parent =
        g::project::object::create("Parent object", nullptr);
    g::project::object_ptr child2 =
        g::project::object::create("Child object 2", parent);

    EXPECT_EQ(child2->parent(), parent);
    EXPECT_EQ(parent->children().size(), 1);
}

TEST(object, remove)
{
    g::project::object_ptr parent =
        g::project::object::create("Parent object", nullptr);
    g::project::object_ptr child =
        g::project::object::create("Child object", parent);

    EXPECT_EQ(child->parent(), parent);
    EXPECT_EQ(parent->children().size(), 1);

    parent->remove_child(child);

    EXPECT_EQ(child->parent(), nullptr);
    EXPECT_EQ(parent->children().size(), 0);
}
