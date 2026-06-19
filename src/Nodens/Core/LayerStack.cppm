/// @file LayerStack.cppm
/// @brief Ordered container of Layer pointers with separate regions for layers and overlays.
/// @ingroup Core

export module Nodens.LayerStack;

import Nodens.Layer;
import std;

export namespace Nodens
{
/// @brief An ordered collection of Layer pointers, split into a layer region and an overlay region.
/// @details Layers are inserted before the overlay boundary (via PushLayer) and overlays
///          are appended to the end (via PushOverlay). This ensures overlays are always
///          updated and rendered after regular layers.
///
///          The LayerStack owns the Layer pointers and deletes them on destruction.
/// @see Layer, Application::PushLayer, Application::PushOverlay
/// @ingroup Core
class LayerStack
{
public:
    LayerStack();

    /// @brief Destructor. Deletes all owned Layer pointers.
    ~LayerStack();

    /// @brief Inserts a layer before the overlay boundary.
    /// @param layer Raw pointer to the layer. Ownership is taken by the LayerStack.
    void PushLayer(Layer* layer);

    /// @brief Appends an overlay after all regular layers.
    /// @param overlay Raw pointer to the overlay. Ownership is taken by the LayerStack.
    void PushOverlay(Layer* overlay);

    /// @brief Removes a regular layer without deleting it.
    /// @param layer The layer to remove. The caller regains ownership.
    void PopLayer(Layer* layer);

    /// @brief Removes an overlay without deleting it.
    /// @param overlay The overlay to remove. The caller regains ownership.
    void PopOverlay(Layer* overlay);

    /// @brief Returns an iterator to the first layer (front-to-back traversal).
    std::vector<Layer*>::iterator begin()
    {
        return m_Layers.begin();
    }

    /// @brief Returns an iterator past the last overlay.
    std::vector<Layer*>::iterator end()
    {
        return m_Layers.end();
    }

private:
    std::vector<Layer*> m_Layers;        ///< Flat list: [layers ... | overlays ...].
    unsigned int m_LayerInsertIndex = 0; ///< Boundary index between layers and overlays.
};
} // namespace Nodens
