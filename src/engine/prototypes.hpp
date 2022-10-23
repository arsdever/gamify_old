
#pragma once

namespace g::project
{

class engine;
using engine_ptr = std::shared_ptr<engine>;
using engine_wptr = std::weak_ptr<engine>;
using engine_uptr = std::unique_ptr<engine>;
using engine_cptr = std::shared_ptr<const engine>;
using engine_cwptr = std::weak_ptr<const engine>;
using engine_cuptr = std::unique_ptr<const engine>;

class clock;
using clock_ptr = std::shared_ptr<clock>;
using clock_wptr = std::weak_ptr<clock>;
using clock_uptr = std::unique_ptr<clock>;
using clock_cptr = std::shared_ptr<const clock>;
using clock_cwptr = std::weak_ptr<const clock>;
using clock_cuptr = std::unique_ptr<const clock>;

} // namespace g::project