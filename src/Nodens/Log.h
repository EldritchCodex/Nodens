#pragma once

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

#include "Core.h"
#include "ndpch.h"

namespace Nodens
{

class Log
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

} // namespace Nodens

#define ND_CORE_ERROR(...) ::Nodens::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ND_CORE_WARN(...)  ::Nodens::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ND_CORE_INFO(...)  ::Nodens::Log::GetCoreLogger()->info(__VA_ARGS__)

#define ND_ERROR(...)      ::Nodens::Log::GetClientLogger()->error(__VA_ARGS__)
#define ND_WARN(...)       ::Nodens::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ND_INFO(...)       ::Nodens::Log::GetClientLogger()->info(__VA_ARGS__)

#ifdef ND_DEBUG
    #define ND_CORE_TRACE(...) ::Nodens::Log::GetCoreLogger()->trace(__VA_ARGS__)
    #define ND_TRACE(...)      ::Nodens::Log::GetClientLogger()->trace(__VA_ARGS__)
#else
    #define ND_CORE_TRACE(...)
    #define ND_TRACE(...)
#endif