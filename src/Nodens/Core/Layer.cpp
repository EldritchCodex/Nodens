/// @file Layer.cpp
/// @brief Implementation of the Layer base class constructor and destructor.
/// @ingroup Core

module Nodens.Layer;

import std;

namespace Nodens
{
Layer::Layer(const std::string& name) : m_DebugName(name)
{
}

Layer::~Layer()
{
}
} // namespace Nodens
