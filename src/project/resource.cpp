#include "project/resource.hpp"

namespace g::project
{

resource::resource(resource_type type)
    : _uuid(common::generate_uuid())
    , _type(type)
{
}

resource::~resource() = default;

common::uuid resource::uuid() const { return _uuid; }

resource_type resource::type() const { return _type; }

} // namespace g::project
