module;

#include <GLFW/glfw3.h>

module Nodens.Input;

import Nodens.Application;
import Nodens.KeyCodes;
import Nodens.MouseButtonCodes;
import std;

namespace Nodens
{

class GlfwInput : public Input
{
protected:
    bool IsKeyPressedImpl(KeyboardKey keycode) override
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state  = glfwGetKey(window, std::to_underlying(keycode));
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool IsMouseButtonPressedImpl(MouseButton buttoncode) override
    {
        auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        auto state  = glfwGetMouseButton(window, std::to_underlying(buttoncode));
        return state == GLFW_PRESS;
    }

    std::pair<float, float> GetMousePositionImpl() override
    {
        auto   window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
        double xPos, yPos;
        glfwGetCursorPos(window, &xPos, &yPos);
        return {(float)xPos, (float)yPos};
    }

    float GetMouseXImpl() override
    {
        return GetMousePositionImpl().first;
    }

    float GetMouseYImpl() override
    {
        return GetMousePositionImpl().second;
    }
};

Input* Input::s_Instance = new GlfwInput();

} // namespace Nodens
