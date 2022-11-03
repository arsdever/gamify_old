#include <stdafx>

#include <project/behavior_registry.hpp>

namespace g::project
{

std::vector<std::string> behavior_registry::available_types()
{
    std::vector<std::string> result;
    for (auto const& [ name, _ ] : _types)
    {
        result.push_back(name);
    }
    return result;
}

void behavior_registry::unregister_type(std::string_view name)
{
    std::erase_if(_types,
                  [ & ](auto const& pair) { return pair.first == name; });
}

creator_mapping_t _types;

} // namespace g::project