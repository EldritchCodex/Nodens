#pragma once

#include "Nodens/Application.h"
#include "Nodens/Input.h"
#include "Nodens/KeyCodes.h"
#include "Nodens/Layer.h"
#include "Nodens/Log.h"
#include "Nodens/MouseButtonCodes.h"
#include "Nodens/TimeStep.h"
#include "Nodens/imgui/ImGuiLayer.h"

// Vendor libraries that users will interact directly with
#include <imgui.h>
#include <implot.h>
#include <implot3d.h>
#include <tracy/Tracy.hpp>

#ifdef MAIN_APPLICATION_FILE
#pragma message("MAIN_APPLICATION_FILE defined")
#include "Nodens/EntryPoint.h"
#endif // MAIN_APPLICATION_FILE