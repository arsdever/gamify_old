
#pragma once

namespace g::engine
{

class engine;
using engine_ptr = std::shared_ptr<engine>;
using engine_wptr = std::weak_ptr<engine>;
using engine_uptr = std::unique_ptr<engine>;
using engine_cptr = std::shared_ptr<const engine>;
using engine_cwptr = std::weak_ptr<const engine>;
using engine_cuptr = std::unique_ptr<const engine>;

} // namespace g::project