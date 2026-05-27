#pragma once

#include "Nodens/Application.h"
#include "Nodens/Events/ApplicationEvent.h"
#include "Nodens/Events/AsyncEventBus.h"
#include "Nodens/Events/Event.h"
#include "Nodens/Events/KeyEvent.h"
#include "Nodens/Events/MouseEvent.h"
#include "Nodens/Input.h"
#include "Nodens/JobSystem.h"
#include "Nodens/KeyCodes.h"
#include "Nodens/Layer.h"
#include "Nodens/Log.h"
#include "Nodens/MouseButtonCodes.h"
#include "Nodens/TimeStep.h"

// Vendor libraries that users will interact directly with
#include <imgui.h>
#include <implot.h>
#include <implot3d.h>
#include <tracy/Tracy.hpp>

#ifdef MAIN_APPLICATION_FILE
#pragma message("MAIN_APPLICATION_FILE defined")
#include "Nodens/EntryPoint.h"
#endif // MAIN_APPLICATION_FILE