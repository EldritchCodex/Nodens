#pragma once

#include <concepts>
#include <memory>

#ifdef _WIN32
    #define ND_PLATFORM_WINDOWS
    #ifdef _WIN64
        #define ND_PLATFORM_WINDOWS_X64
    #else
        #define ND_PLATFORM_WINDOWS_X86
    #endif
#elif defined(__linux__)
    #define ND_PLATFORM_LINUX
#else
    #error "Unsupported platform!"
#endif

#define ND_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Nodens
{
/// @brief Generates a bitfield mask at compile-time.
/// @details It uses 'consteval' to ensure zero runtime cost, enforcing
/// execution during compilation.
/// @tparam T The integer type of the result (defaults to int, use uint64_t for
/// large flags).
/// @param x The bit position to shift.
/// @return The shifted bit mask.
template <std::integral T = int> consteval T Bit(T x)
{
    return T(1) << x;
}

} // namespace Nodens

#ifdef ND_ENABLE_ASSERTS
#define ND_ASSERT(x, ...)                                                                                              \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            ND_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                            \
            __debugbreak();                                                                                            \
        }                                                                                                              \
    }
#define ND_CORE_ASSERT(x, ...)                                                                                         \
    {                                                                                                                  \
        if (!(x))                                                                                                      \
        {                                                                                                              \
            ND_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__);                                                       \
            __debugbreak();                                                                                            \
        }                                                                                                              \
    }

#else
#define ND_ASSERT(x, ...)
#define ND_CORE_ASSERT(x, ...)
#endif
