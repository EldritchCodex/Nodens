#pragma once

#include "Nodens/Application.h"
#include "Nodens/Core/TimeStep.h"
#include "Nodens/Input.h"
#include "Nodens/KeyCodes.h"
#include "Nodens/Layer.h"
#include "Nodens/Log.h"
#include "Nodens/MouseButtonCodes.h"
#include "Nodens/Profiling.h"
#include "Nodens/imgui/ImGuiLayer.h"

//------Renderer-------------------
#include "Nodens/Renderer/Buffer.h"
#include "Nodens/Renderer/DirectionalLight.h"
#include "Nodens/Renderer/OrthographicCamera.h"
#include "Nodens/Renderer/PerspectiveCamera.h"
#include "Nodens/Renderer/PointLight.h"
#include "Nodens/Renderer/RenderCommand.h"
#include "Nodens/Renderer/Renderer.h"
#include "Nodens/Renderer/Shader.h"
#include "Nodens/Renderer/VertexArray.h"
//----------------------------------

#ifdef MAIN_APPLICATION_FILE
#pragma message("MAIN_APPLICATION_FILE defined")
#include "Nodens/EntryPoint.h"
#endif  // MAIN_APPLICATION_FILE