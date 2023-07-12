#pragma once

#include <memory>

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

class mesh_asset;
using mesh_asset_ptr = std::shared_ptr<mesh_asset>;
using mesh_asset_wptr = std::weak_ptr<mesh_asset>;
using mesh_asset_uptr = std::unique_ptr<mesh_asset>;
using mesh_asset_cptr = std::shared_ptr<const mesh_asset>;
using mesh_asset_cwptr = std::weak_ptr<const mesh_asset>;
using mesh_asset_cuptr = std::unique_ptr<const mesh_asset>;

class asset;
using asset_ptr = std::shared_ptr<asset>;
using asset_wptr = std::weak_ptr<asset>;
using asset_uptr = std::unique_ptr<asset>;
using asset_cptr = std::shared_ptr<const asset>;
using asset_cwptr = std::weak_ptr<const asset>;
using asset_cuptr = std::unique_ptr<const asset>;

class asset_manager;
using asset_manager_ptr = std::shared_ptr<asset_manager>;
using asset_manager_wptr = std::weak_ptr<asset_manager>;
using asset_manager_uptr = std::unique_ptr<asset_manager>;
using asset_manager_cptr = std::shared_ptr<const asset_manager>;
using asset_manager_cwptr = std::weak_ptr<const asset_manager>;
using asset_manager_cuptr = std::unique_ptr<const asset_manager>;

class component;
using component_ptr = std::shared_ptr<component>;
using component_wptr = std::weak_ptr<component>;
using component_uptr = std::unique_ptr<component>;
using component_cptr = std::shared_ptr<const component>;
using component_cwptr = std::weak_ptr<const component>;
using component_cuptr = std::unique_ptr<const component>;

class shader_asset;
using shader_asset_ptr = std::shared_ptr<shader_asset>;
using shader_asset_wptr = std::weak_ptr<shader_asset>;
using shader_asset_uptr = std::unique_ptr<shader_asset>;
using shader_asset_cptr = std::shared_ptr<const shader_asset>;
using shader_asset_cwptr = std::weak_ptr<const shader_asset>;
using shader_asset_cuptr = std::unique_ptr<const shader_asset>;

class material_asset;
using material_asset_ptr = std::shared_ptr<material_asset>;
using material_asset_wptr = std::weak_ptr<material_asset>;
using material_asset_uptr = std::unique_ptr<material_asset>;
using material_asset_cptr = std::shared_ptr<const material_asset>;
using material_asset_cwptr = std::weak_ptr<const material_asset>;
using material_asset_cuptr = std::unique_ptr<const material_asset>;

} // namespace g::project
