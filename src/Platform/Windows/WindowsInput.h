#pragma once

#include "Nodens/Input.h"

namespace Nodens
{
/// @brief Windows specific implementation of the Input interface.
class WindowsInput : public Input
{
protected:
    /// @brief Checks if a specific key is pressed.
    /// @param keycode The key to check.
    /// @return True if the key is pressed, false otherwise.
    virtual bool IsKeyPressedImpl(KeyboardKey keycode) override;

    /// @brief Checks if a specific mouse button is pressed.
    /// @param button The mouse button to check.
    /// @return True if the mouse button is pressed, false otherwise.
    virtual bool IsMouseButtonPressedImpl(MouseButton buttoncode) override;

    /// @brief Gets the current mouse position.
    /// @return A pair of floats representing the x and y coordinates of the mouse.
    virtual std::pair<float, float> GetMousePositionImpl() override;

    /// @brief Gets the current x-coordinate of the mouse.
    /// @return The x-coordinate of the mouse.
    virtual float GetMouseXImpl() override;

    /// @brief Gets the current y-coordinate of the mouse.
    /// @return The y-coordinate of the mouse.
    virtual float GetMouseYImpl() override;
};
} // namespace Nodens