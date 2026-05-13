#include "LayerStack.h"

namespace Aozora {

	void LayerStack::addLayer(Layer* layer)
	{
		m_layerStack.insert(m_layerStack.begin(), layer);
		layer->onAttach();
	}

	void LayerStack::removeLayer(Layer* layer)
	{
	}

	void LayerStack::addOverlay(Layer* layer)
	{
		m_layerStack.push_back(layer); // for now
		layer->onAttach();
	}

	void LayerStack::removeOverlay(Layer* layer)
	{
	}
}
