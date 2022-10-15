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

} // namespace g::project