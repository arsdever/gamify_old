#pragma once

#include <chrono>
#include <list>
#include <string>

#include "common/uuid.hpp"
#include "project/concepts.hpp"
#include "project/resource.hpp"

namespace g::project
{

class object;

class scene : public resource
{
protected:
    scene();

public:
    virtual ~scene();
    static std::shared_ptr<scene> create(std::string_view name);

    std::string name() const;
    std::list<common::uuid> const& objects() const;

    void add_object(std::shared_ptr<object> object);
    void remove_object(std::shared_ptr<object> object);

private:
    std::string _name;
    std::list<common::uuid> _objects_uuid;
};

template <>
struct resource_type_traits<scene>
{
    static constexpr resource_type type = resource_type::scene;
};

} // namespace g::project
