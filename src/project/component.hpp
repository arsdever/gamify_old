#pragma once

#include "common/uuid.hpp"
#include "project/object.hpp"
#include "project/resource.hpp"

namespace g::rendering
{
struct gizmo_renderer;
} // namespace g::rendering

namespace g::project
{

class transform_component;

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

    std::shared_ptr<transform_component> transform() const;

    template <typename T>
    std::shared_ptr<T> get_component() const
    {
        return std::dynamic_pointer_cast<T>(object()->get_component(T::type()));
    }

    virtual void draw_gizmos(rendering::gizmo_renderer& renderer) const;

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
