#pragma once

#include <cstdint>
#include <ostream>
#include <utility>

namespace Nodens
{
/// @brief      Mouse button codes.
///             These mouse button codes are based on the button codes from glfw3.h.
enum class MouseButton : uint16_t
{
    // From glfw3.h
    Button0 = 0, ///< The 0 mouse button.
    Button1 = 1, ///< The 1 mouse button.
    Button2 = 2, ///< The 2 mouse button.
    Button3 = 3, ///< The 3 mouse button.
    Button4 = 4, ///< The 4 mouse button.
    Button5 = 5, ///< The 5 mouse button.
    Button6 = 6, ///< The 6 mouse button.
    Button7 = 7, ///< The 7 mouse button.

    ButtonLast = Button7, ///< The last mouse button.
    Left       = Button0, ///< The left mouse button.
    Right      = Button1, ///< The right mouse button.
    Middle     = Button2  ///< The middle mouse button.
};

/// @brief      Overload of the << operator for the Mouse enum.
///             This allows printing Mouse codes directly to output streams (e.g., for logging).
/// @param      os  The output stream.
/// @param[in]  mouseCode The mouse button code to print.
/// @return     The output stream.
inline std::ostream& operator<<(std::ostream& os, MouseButton mouseCode)
{
    os << std::to_underlying(mouseCode);
    return os;
}
} // namespace Nodens