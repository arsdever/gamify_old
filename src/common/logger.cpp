#include "common/logger.hpp"

#include "spdlog/cfg/argv.h"
#include "spdlog/cfg/env.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace g::common
{

void configure_logger(int argc, char** argv)
{
    spdlog::cfg::load_env_levels();
    spdlog::cfg::load_argv_levels(argc, argv);
}

logger_ptr get_logger(std::string_view name, bool is_dummy)
{
    std::string name_str { name };
    auto logger = spdlog::get(name_str);
    if (logger)
        return logger;

    logger = std::make_shared<spdlog::logger>(name_str);
    logger->set_level(spdlog::level::info);

    if (!is_dummy)
    {
        static auto sink =
            std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        logger->sinks().push_back(sink);
    }

    spdlog::register_logger(logger);

    return logger;
}

} // namespace g::common
