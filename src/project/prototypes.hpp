#pragma once

namespace g::project
{

class project;
using project_ptr = std::shared_ptr<project>;
using project_wptr = std::weak_ptr<project>;
using project_uptr = std::unique_ptr<project>;
using project_cptr = std::shared_ptr<const project>;
using project_cwptr = std::weak_ptr<const project>;
using project_cuptr = std::unique_ptr<const project>;

class scene;
using scene_ptr = std::shared_ptr<scene>;
using scene_wptr = std::weak_ptr<scene>;
using scene_uptr = std::unique_ptr<scene>;
using scene_cptr = std::shared_ptr<const scene>;
using scene_cwptr = std::weak_ptr<const scene>;
using scene_cuptr = std::unique_ptr<const scene>;

class object;
using object_ptr = std::shared_ptr<object>;
using object_wptr = std::weak_ptr<object>;
using object_uptr = std::unique_ptr<object>;
using object_cptr = std::shared_ptr<const object>;
using object_cwptr = std::weak_ptr<const object>;
using object_cuptr = std::unique_ptr<const object>;

class asset_manager;
using asset_manager_ptr = std::shared_ptr<asset_manager>;
using asset_manager_wptr = std::weak_ptr<asset_manager>;
using asset_manager_uptr = std::unique_ptr<asset_manager>;
using asset_manager_cptr = std::shared_ptr<const asset_manager>;
using asset_manager_cwptr = std::weak_ptr<const asset_manager>;
using asset_manager_cuptr = std::unique_ptr<const asset_manager>;

} // namespace g::project
