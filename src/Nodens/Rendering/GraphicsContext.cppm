/// @file GraphicsContext.cppm
/// @brief Abstract interface for a graphics rendering context.
/// @ingroup Rendering

export module Nodens.GraphicsContext;

export namespace Nodens
{
/// @brief Abstract base class representing a GPU rendering context.
/// @details A GraphicsContext encapsulates the initialization and buffer-swapping logic
///          for a specific graphics API (e.g., OpenGL, Vulkan). The concrete implementation
///          is created by the platform Window during initialization.
/// @see OpenGLContext, Window
/// @ingroup Rendering
class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;

    /// @brief Initializes the graphics context (e.g., loads function pointers, sets GL state).
    virtual void Init()        = 0;

    /// @brief Swaps the front and back framebuffers, presenting the rendered frame.
    virtual void SwapBuffers() = 0;
};
} // namespace Nodens
