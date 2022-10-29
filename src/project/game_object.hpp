#pragma once

#include "project/prototypes.hpp"

#include "core/pointer_traits.hpp"
#include "project/object.hpp"

namespace g::project
{

class game_object : public object
{
public:
    game_object();
    virtual ~game_object();

    template <typename behavior_t>
        requires std::derived_from<behavior_t, behavior>
    typename pointer_traits<behavior_t>::sptr get_behavior()
    {
        auto& it = std::find_if(_behaviors.begin(),
                                _behaviors.end(),
                                [](auto const& behavior) {
            return std::dynamic_pointer_cast<behavior_t>(behavior) != nullptr;
        });
        return it == _behaviors.end()
                   ? nullptr
                   : std::static_pointer_cast<behavior_t>(*it);
    }

    template <typename behavior_t>
        requires std::derived_from<behavior_t, behavior> bool
    add_behavior(typename pointer_traits<behavior_t>::sptr& pointer)
    {
        auto& it = std::find_if(_behaviors.begin(),
                                _behaviors.end(),
                                [](auto const& behavior) {
            return std::dynamic_pointer_cast<behavior_t>(behavior) != nullptr;
        });

        if (it != _behaviors.end())
            return false;

        _behaviors.emplace_back(std::make_shared<behavior_t>());
        return true;
    }

    std::list<behavior_ptr> behaviors() const { return _behaviors; }

private:
    std::list<behavior_ptr> _behaviors;
};

} // namespace g::project