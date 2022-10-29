#pragma once

namespace g::project
{

class project;
using project_ptr = std::shared_ptr<project>;
using project_wptr = std::weak_ptr<project>;
using project_uptr = std::unique_ptr<project>;

class scene;
using scene_ptr = std::shared_ptr<scene>;
using scene_wptr = std::weak_ptr<scene>;
using scene_uptr = std::unique_ptr<scene>;

class object;
using object_ptr = std::shared_ptr<object>;
using object_wptr = std::weak_ptr<object>;
using object_uptr = std::unique_ptr<object>;

class game_object;
using game_object_ptr = std::shared_ptr<game_object>;
using game_object_wptr = std::weak_ptr<game_object>;
using game_object_uptr = std::unique_ptr<game_object>;

class behavior;
using behavior_ptr = std::shared_ptr<behavior>;
using behavior_wptr = std::weak_ptr<behavior>;
using behavior_uptr = std::unique_ptr<behavior>;

} // namespace g::project