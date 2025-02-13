#pragma once

#include <memory>

#ifdef ND_DEBUG
#define ND_ENABLE_ASSERTS
#endif // ND_DEBUG

#ifdef ND_ENABLE_ASSERTS
#define ND_ASSERT(x, ...)                             \
  {                                                   \
    if (!(x)) {                                       \
      ND_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      __debugbreak();                                 \
    }                                                 \
  }
#define ND_CORE_ASSERT(x, ...)                             \
  {                                                        \
    if (!(x)) {                                            \
      ND_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
      __debugbreak();                                      \
    }                                                      \
  }

#else
#define ND_ASSERT(x, ...)
#define ND_CORE_ASSERT(x, ...)

#endif  // ND_ENABLE_ASSERTS

#define BIT(x) (1 << x)  // Shift 1 to the left by x places (ex 1 << 2 = 100)
#define ND_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

// std::bind(Application::OnEvent*, Application*, std::placeholders::_1)

namespace Nodens {
template <typename T>
using Scope = std::unique_ptr<T>;

template <typename T>
using Ref = std::shared_ptr<T>;

}  // namespace Nodens