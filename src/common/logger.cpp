#include <common/logger.hpp>
#include <spdlog/logger.hpp>

namespace g::common
{

logger_ptr get_logger(std::string_view name)
{
	auto logger = spdlog::get(name);
	if (logger)
        return logger;

    logger = std::make_shared<spdlog::logger>(name);
    return logger;
}

} // namespace g::common