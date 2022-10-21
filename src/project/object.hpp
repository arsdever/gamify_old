#pragma once

#include <project/prototypes.hpp>

#include <boost/signals2.hpp>

namespace g::project
{

class object : public std::enable_shared_from_this<object>
{
public:
    object(std::string_view name, object_ptr parent = nullptr);
    virtual ~object();

    std::string name();

    void make_parent_of(object_ptr child);
    void add_child(object_ptr child);
    template <typename container_t>
        requires std::same_as<typename container_t::element_type, object_ptr>
    void add_children(container_t children)
    {
        std::for_each(std::begin(children),
                      std::end(children),
                      [ this ](object_ptr child) { add_child(child); });
    }

    void make_child_of(object_ptr parent);
    void remove_child(object_ptr child);

    std::list<object_ptr> const& children() const;
    object_ptr parent() const;

#pragma region signals
public:
    boost::signals2::signal<void(object_ptr)> on_parent_changed;
    boost::signals2::signal<void()> on_children_list_changed;
#pragma endregion

private:
    object_wptr _parent;
    std::string _name;
    std::list<object_ptr> _children;
};

} // namespace g::project
