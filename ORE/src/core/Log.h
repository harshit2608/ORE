#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>

namespace ORE
{
    class Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
    };
} // namespace ORE

// Core log macros
#define ORE_CORE_TRACE(...) ::ORE::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ORE_CORE_INFO(...) ::ORE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define ORE_CORE_WARN(...) ::ORE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ORE_CORE_ERROR(...) ::ORE::Log::GetCoreLogger()->error(__VA_ARGS__)
#define ORE_CORE_CRITICAL(...) ::ORE::Log::GetCoreLogger()->critical(__VA_ARGS__)
