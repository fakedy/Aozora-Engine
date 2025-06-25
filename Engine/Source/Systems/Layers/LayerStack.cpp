#include "LayerStack.h"

namespace Aozora {

	void LayerStack::addLayer(Layer* layer)
	{
		m_layerStack.insert(m_layerStack.begin(), layer);
	}

	void LayerStack::removeLayer(Layer* layer)
	{
	}

	void LayerStack::addOverlay(Layer* layer)
	{
		m_layerStack.push_back(layer); // for now
	}

	void LayerStack::removeOverlay(Layer* layer)
	{
	}
}
