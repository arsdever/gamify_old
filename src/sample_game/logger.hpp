#pragma once

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace g::sample_game
{

std::shared_ptr<spdlog::logger> logger()
{
    static auto logger = std::make_shared<spdlog::logger>("sample_game");
    logger->sinks().push_back(
        std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    return logger;
}

} // namespace g::sample_game