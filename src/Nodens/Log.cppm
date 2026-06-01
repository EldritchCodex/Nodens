module;

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

export module Nodens.Log;

import std;

export namespace Nodens
{
inline spdlog::logger& CoreLogger()
{
    static std::shared_ptr<spdlog::logger> coreLogger = []()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        std::shared_ptr<spdlog::logger> logger = spdlog::get("NODENS");
        if (!logger)
        {
            logger = spdlog::stdout_color_mt("NODENS");
            logger->set_level(spdlog::level::trace);
        }

        return logger;
    }();

    return *coreLogger;
}

inline spdlog::logger& ClientLogger()
{
    static std::shared_ptr<spdlog::logger> clientLogger = []()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");

        std::shared_ptr<spdlog::logger> logger = spdlog::get("APP");
        if (!logger)
        {
            logger = spdlog::stdout_color_mt("APP");
            logger->set_level(spdlog::level::trace);
        }

        return logger;
    }();

    return *clientLogger;
}

inline void InitializeLogging()
{
    (void)CoreLogger();
    (void)ClientLogger();
}

[[noreturn]] inline void FatalCore(std::string_view message)
{
    CoreLogger().error("{}", message);
    std::terminate();
}
} // namespace Nodens
