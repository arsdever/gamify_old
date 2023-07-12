#pragma once

#include "spdlog/spdlog.h"

namespace g::common
{

using logger_ptr = std::shared_ptr<spdlog::logger>;

void configure_logger(int argc, char** argv);

/**
 * @brief Get the logger object
 *
 * Creates a new logger and registers it if it doesn't exist. The user can
 * specify @c is_dummy flag in order to make the logger not print anything.
 *
 * @param name the name of the logger
 * @param is_dummy whether the logger is dummy and should not print anything
 *
 * @return logger_ptr the logger
 */
logger_ptr get_logger(std::string_view name, bool is_dummy = false);

} // namespace g::common
