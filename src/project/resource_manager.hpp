#pragma once

#include <memory>
#include <unordered_map>

#include "common/profiler.hpp"
#include "common/uuid.hpp"
#include "project/concepts.hpp"
#include "project/resource.hpp"

namespace g::project
{

class resource_manager : public std::enable_shared_from_this<resource_manager>
{
protected:
    resource_manager();

public:
    ~resource_manager();

    static std::shared_ptr<resource_manager>
    init(std::shared_ptr<resource_manager> instance = nullptr);
    static std::shared_ptr<resource_manager> get();
    static void deinit();

    void register_resource(std::shared_ptr<resource> resource);
    void unregister_resource(std::shared_ptr<resource> resource);
    void unregister_resource(common::uuid uuid);

    std::shared_ptr<resource> get_resource(common::uuid uuid) const;

    template <is_resource T>
    std::shared_ptr<T> get_resource(common::uuid uuid) const
    {
        common::profile(__FUNCTION__);
        auto resource = get_resource(uuid);
        if (resource == nullptr)
            return nullptr;

        if (resource->type() != resource_type_traits<T>::type)
            throw std::runtime_error("Resource type mismatch");

        return std::static_pointer_cast<T>(get_resource(uuid));
    }

    template <is_resource T>
    static std::shared_ptr<T> get_resource_static(common::uuid uuid)
    {
        common::profile(__FUNCTION__);
        return get()->get_resource<T>(uuid);
    }

    template <is_resource T, typename... ARGS>
    std::shared_ptr<T> create(ARGS&&... args)
    {
        common::profile(__FUNCTION__);
        auto resource = std::make_shared<T>(std::forward(args)...);
        register_resource(resource);
        return resource;
    }

private:
    static std::shared_ptr<resource_manager> _instance;
    std::unordered_map<common::uuid, std::shared_ptr<resource>> _resources;
};

} // namespace g::project
