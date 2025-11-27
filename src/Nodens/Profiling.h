#pragma once

// Check if Profiling is enabled (Defined in CMakeLists.txt)
#ifdef TRACY_ENABLE
#include <cstring>
#include <tracy/Tracy.hpp>

// 1. Standard Scope
#define ND_PROFILE_ZONE_SCOPED ZoneScoped

// 2. Static Named Scope (String Literal)
#define ND_PROFILE_ZONE_NAMED(name) ZoneScopedN(name)

// 3. Dynamic Named Scope (Runtime String)
#define ND_PROFILE_ZONE_NAMED_DYNAMIC(name)                                                                            \
    ZoneScoped;                                                                                                        \
    ZoneName(name, strlen(name))

// 4. Frame Marker
#define ND_PROFILE_FRAME_MARK FrameMark

// 5. Thread Naming
#define ND_PROFILE_SET_THREAD_NAME(name) tracy::SetThreadName(name)

// 6. Locks (Mutex Wrappers)
// Replaces: std::mutex m_Mutex;
// With:     ND_PROFILE_LOCKABLE(std::mutex, m_Mutex);
#define ND_PROFILE_LOCKABLE(type, varname) TracyLockable(type, varname)
#define ND_PROFILE_LOCKABLE_NAMED(type, varname, name) TracyLockableN(type, varname, name)
#define ND_PROFILE_LOCK_MARK(varname) LockMark(varname)

// 7. Plots (Graphs)
// e.g., ND_PROFILE_PLOT("Queue Size", (int64_t)size);
#define ND_PROFILE_PLOT(name, val) TracyPlot(name, val)
#define ND_PROFILE_PLOT_CONFIG(name, type, step, fill, color) TracyPlotConfig(name, type, step, fill, color)

#else
// If profiling is disabled, these macros do nothing or decay to standard C++.
#define ND_PROFILE_ZONE_SCOPED
#define ND_PROFILE_ZONE_NAMED(name)
#define ND_PROFILE_ZONE_NAMED_DYNAMIC(name)
#define ND_PROFILE_FRAME_MARK
#define ND_PROFILE_SET_THREAD_NAME(name)

// Decays to a standard variable declaration: 'type varname;'
#define ND_PROFILE_LOCKABLE(type, varname) type varname
#define ND_PROFILE_LOCKABLE_NAMED(type, varname, name) type varname
#define ND_PROFILE_LOCK_MARK(varname)

#define ND_PROFILE_PLOT(name, val)
#define ND_PROFILE_PLOT_CONFIG(name, type, step, fill, color)
#endif