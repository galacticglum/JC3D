#include <LayerStack.h>

LayerStack::LayerStack(): m_LayerInsertIndex(0)
{
}

LayerStack::~LayerStack()
{
	// Destroy every layer
	for (auto layer : m_Layers)
	{
		delete layer;
	}
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(Layer* layer)
{
	m_Layers.emplace_back(layer);
}

void LayerStack::PopLayer(Layer* layer)
{
	// Find the layer in the stack
	const auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

	// If we found the layer in the stack, remove it from the stack
	// and update the top iterator.
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		--m_LayerInsertIndex;
	}
}

void LayerStack::PopOverlay(Layer* layer)
{
	// Popping an overlay layer works almost exactly the same as popping a regular layer
	// except that we don't update the top iterator (since an overlay layer can be in the middle
	// of the stack---and that is okay!).

	const auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
	}
}



