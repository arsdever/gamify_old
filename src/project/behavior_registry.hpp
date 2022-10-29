#pragma once

#include <project/prototypes.hpp>

namespace g::project
{

class behavior_registry
{
public:
    template <typename T>
    static void register_type(std::string_view name)
    {
        _types[ name ] = []() { return T::create(); };
    }

    static behavior_ptr create(std::string_view name)
    {
        return _types[ name ]();
    }

private:
    static std::map<std::string, std::function<behavior_ptr()>> _types;
};

} // namespace g::project