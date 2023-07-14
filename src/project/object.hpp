#pragma once

#include "project/concepts.hpp"
#include "project/resource.hpp"

namespace g::project
{

class component;
class transform_component;

class object : public resource
{
protected:
    object();

public:
    virtual ~object();

    static std::shared_ptr<object>
    create(std::string_view name, std::shared_ptr<object> parent = nullptr);

    std::string name();
    std::list<common::uuid> const& children_uuid() const;

    common::uuid parent_uuid() const;
    std::shared_ptr<object> parent() const;

    void add_child(std::shared_ptr<object> child);
    void remove_child(std::shared_ptr<object> child);
    void change_parent(std::shared_ptr<object> parent);

    std::shared_ptr<transform_component> transform() const;

    template <is_component T, typename... ARGS>
    std::shared_ptr<T> add_component(ARGS&&... args)
    {
        auto component =
            T::create(std::forward(args)...,
                      std::static_pointer_cast<object>(shared_from_this()));
        add_component(component);
        return component;
    }
    std::shared_ptr<component> get_component(std::string_view classname);

    template <is_component T>
    std::shared_ptr<T> get_component()
    {
        return std::dynamic_pointer_cast<T>(get_component(T::type()));
    }

private:
    void add_component(std::shared_ptr<component> component);

    void add_child_uuid(common::uuid child_uuid);
    void remove_child_uuid(common::uuid child_uuid);
    void change_parent_uuid(common::uuid parent_uuid);

private:
    std::string _name;
    common::uuid _parent_uuid;
    std::list<common::uuid> _children_uuid;
    std::list<common::uuid> _components_uuid;
    std::shared_ptr<transform_component> _transform;
};

template <>
struct resource_type_traits<object>
{
    static constexpr resource_type type = resource_type::object;
};

} // namespace g::project
