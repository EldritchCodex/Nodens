#pragma once

#include <Tracy.hpp>
#include <cstring>

// 1. Standard Scope (Function Name)
// Usage: ND_PROFILE_ZONE_SCOPED;
#define ND_PROFILE_ZONE_SCOPED ZoneScoped

// 2. Static Named Scope (String Literals ONLY)
// Usage: ND_PROFILE_ZONE_NAMED("Physics System");
// Faster than dynamic because the name is baked into the binary.
#define ND_PROFILE_ZONE_NAMED(name) ZoneScopedN(name)

// 3. Dynamic Named Scope (Runtime Strings)
// Usage: ND_PROFILE_ZONE_DYNAMIC(event->GetName());
// This combines the creation and naming into one call.
// NOTE: Do not use this inside a single-line if statement without braces!
#define ND_PROFILE_ZONE_NAMED_DYNAMIC(name) \
    ZoneScoped; \
    ZoneName(name, strlen(name))

// 4. Mark the end of a frame
#define ND_PROFILE_FRAME_MARK FrameMark