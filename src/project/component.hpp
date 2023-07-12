#pragma once

#include "common/uuid.hpp"
#include "project/object.hpp"
#include "project/resource.hpp"

namespace g::project
{

class component : public resource
{
protected:
    component(std::string_view classname, std::shared_ptr<class object> object);

public:
    virtual ~component() = 0;

    std::string const& classname() const;
    common::uuid object_uuid() const;
    std::shared_ptr<class object> object() const;
    bool enabled() const;

    void set_enabled(bool enabled);
    void enable();
    void disable();

    template <typename T>
    std::shared_ptr<T> get_component()
    {
        return std::dynamic_pointer_cast<T>(object()->get_component(T::type()));
    }

protected:
    std::string _classname;
    common::uuid _object_uuid;
    bool _enabled = true;
};

template <>
struct resource_type_traits<component>
{
    static constexpr resource_type type = resource_type::component;
};

} // namespace g::project
