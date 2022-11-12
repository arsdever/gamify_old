#pragma once

namespace spdlog
{
class logger;
} // namespace spdlog

namespace g::common
{

using logger_ptr = std::shared_ptr<spdlog::logger>;

logger_ptr get_logger(std::string_view name);

} // namespace g::common