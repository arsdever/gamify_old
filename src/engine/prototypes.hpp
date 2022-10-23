
#pragma once

namespace g::project
{

class clock;
using clock_ptr = std::shared_ptr<clock>;
using clock_wptr = std::weak_ptr<clock>;
using clock_uptr = std::unique_ptr<clock>;
using clock_cptr = std::shared_ptr<const clock>;
using clock_cwptr = std::weak_ptr<const clock>;
using clock_cuptr = std::unique_ptr<const clock>;

} // namespace g::project