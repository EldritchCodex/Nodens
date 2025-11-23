#pragma once

// Check if Profiling is enabled (Defined in CMakeLists.txt)
#ifdef TRACY_ENABLE
    #include <Tracy.hpp>
    #include <cstring>

    // 1. Standard Scope
    #define ND_PROFILE_ZONE_SCOPED ZoneScoped

    // 2. Static Named Scope (String Literal)
    #define ND_PROFILE_ZONE_NAMED(name) ZoneScopedN(name)

    // 3. Dynamic Named Scope (Runtime String)
    #define ND_PROFILE_ZONE_NAMED_DYNAMIC(name) \
        ZoneScoped; \
        ZoneName(name, strlen(name))

    // 4. Frame Marker
    #define ND_PROFILE_FRAME_MARK FrameMark

    // 5. Thread Naming
    #define ND_PROFILE_SET_THREAD_NAME(name) tracy::SetThreadName(name)

#else
    // If profiling is disabled, these macros do nothing.
    #define ND_PROFILE_ZONE_SCOPED
    #define ND_PROFILE_ZONE_NAMED(name)
    #define ND_PROFILE_ZONE_NAMED_DYNAMIC(name)
    #define ND_PROFILE_FRAME_MARK
    #define ND_PROFILE_SET_THREAD_NAME(name)
#endif