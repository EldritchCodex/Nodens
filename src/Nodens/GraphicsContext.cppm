export module Nodens.GraphicsContext;

export namespace Nodens
{
class GraphicsContext
{
public:
    virtual ~GraphicsContext() = default;

    virtual void Init()        = 0;
    virtual void SwapBuffers() = 0;
};
} // namespace Nodens
