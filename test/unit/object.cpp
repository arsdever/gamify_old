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

TEST(object, signals)
{
    int children_change_count = 0;
    int parent_change_count = 0;
    auto children_list_change_notifier = [&]() { children_change_count++; };
    auto parent_change_notifier = [&]([[maybe_unused]] g::project::object_ptr obj) { parent_change_count++; };
    g::project::object_ptr parent =
        g::project::object::create("Parent", nullptr);
    g::project::object_ptr child =
        g::project::object::create("Child", nullptr);

    parent->signal_children_list_changed.connect(children_list_change_notifier);
    child->signal_parent_changed.connect(parent_change_notifier);

    parent->add_child(child);
    EXPECT_EQ(children_change_count, 1);
    EXPECT_EQ(parent_change_count, 1);


    g::project::object_ptr new_parent =
        g::project::object::create("New Parent", nullptr);
    new_parent->add_child(child);
    EXPECT_EQ(children_change_count, 2); // Removed 'child' from children. 
    EXPECT_EQ(parent_change_count, 3);  //Removed old parent. Added new parent.

}
