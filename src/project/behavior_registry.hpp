#pragma once

#include <project/prototypes.hpp>

namespace g::project
{

using creator_mapping_t =
    std::unordered_map<std::string, std::function<behavior_ptr(object_wptr)>>;

class behavior_registry
{
public:
    std::vector<std::string> available_types();

    template <typename T>
    void register_type(std::string_view name)
    {
        auto it = _types.emplace(std::string { name },
                                 [](object_wptr object)
                                 { return T::create(object); });
    }

    void unregister_type(std::string_view name);

    behavior_ptr create(std::string_view name, object_wptr object)
    {
        return _types[ std::string { name } ](object);
    }

    inline static behavior_registry& instance()
    {
        static behavior_registry instance;
        return instance;
    }

    creator_mapping_t _types;
};

} // namespace g::project