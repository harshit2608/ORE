#pragma once

#include "PlatformDetection.h"

#ifdef ORE_DEBUG
#if defined(ORE_PLATFORM_WINDOWS)
#define ORE_DEBUGBREAK() __debugbreak()
#elif defined(ORE_PLATFORM_LINUX)
#include <signal.h>
#define ORE_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define ORE_ENABLE_ASSERTS
#else
#define ORE_DEBUGBREAK()
#endif

#ifdef ORE_ENABLE_ASSERTS
#define ORE_ASSERT(x, ...)                                   \
    {                                                        \
        if (!(x))                                            \
        {                                                    \
            ORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            ORE_DEBUGBREAK();                                \
        }                                                    \
    }
#define ORE_CORE_ASSERT(x, ...)                                   \
    {                                                             \
        if (!(x))                                                 \
        {                                                         \
            ORE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            ORE_DEBUGBREAK();                                     \
        }                                                         \
    }
#else
#define ORE_ASSERT(x, ...)
#define ORE_CORE_ASSERT(x, ...)
#endif

namespace ORE
{
    template <typename T>
    using Scope = std::unique_ptr<T>;
    template <typename T, typename... Args>
    constexpr Scope<T> CreateScope(Args &&...args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template <typename T>
    using Ref = std::shared_ptr<T>;
    template <typename T, typename... Args>
    constexpr Ref<T> CreateRef(Args &&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

}