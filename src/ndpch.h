#pragma once

// --- Standard Library ---
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>

// --- Profiling ---
#include <tracy/Tracy.hpp>

// --- Local Engine Headers ---
#include "Nodens/Log.h"

// --- Platform Specific ---
#ifdef ND_PLATFORM_WINDOWS
    #include <Windows.h>
#endif