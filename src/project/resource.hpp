#pragma once

#include <memory>
#include <unordered_map>

#include "common/uuid.hpp"
#include "project/concepts.hpp"

namespace g::project
{

enum class resource_type
{
    asset,
    scene,
    object,
    component
};

template <is_resource T>
struct resource_type_traits;

class resource : public std::enable_shared_from_this<resource>
{
protected:
    resource(resource_type type);

public:
    virtual ~resource();

    common::uuid uuid() const;
    resource_type type() const;

protected:
    common::uuid _uuid;
    resource_type _type;
};

} // namespace g::project
