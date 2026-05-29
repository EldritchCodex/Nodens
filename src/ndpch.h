#pragma once

// --- Standard Library ---
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

// --- Profiling ---
#include <tracy/Tracy.hpp>

// --- Platform Specific ---
#ifdef ND_PLATFORM_WINDOWS
#include <Windows.h>
#endif