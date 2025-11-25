#include "WindowsInput.h"

#include "Nodens/Application.h"
#include "ndpch.h"

#include <GLFW/glfw3.h>
#include <utility> // Required for std::to_underlying (C++23)
                   // converts the strongly typed enum to uint16_t/int

namespace Nodens
{

Input* Input::s_Instance = new WindowsInput();

bool WindowsInput::IsKeyPressedImpl(KeyboardKey keycode)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state  = glfwGetKey(window, std::to_underlying(keycode));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool WindowsInput::IsMouseButtonPressedImpl(MouseButton buttoncode)
{
    auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    auto state  = glfwGetMouseButton(window, std::to_underlying(buttoncode));
    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::GetMousePositionImpl()
{
    auto   window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return {(float)xPos, (float)yPos};
}

float WindowsInput::GetMouseXImpl()
{
    return GetMousePositionImpl().first;
}

float WindowsInput::GetMouseYImpl()
{
    return GetMousePositionImpl().second;
}

} // namespace Nodens