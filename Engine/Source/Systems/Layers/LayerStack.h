#pragma once
#include <vector>
#include <deque>
#include "Layer.h"

namespace Aozora {

	class LayerStack
	{

	public:

		void addLayer(Layer* layer);

		void removeLayer(Layer* layer);

		void addOverlay(Layer* layer);

		void removeOverlay(Layer* layer);

		std::vector<Layer*>::iterator begin() { return m_layerStack.begin(); }
		std::vector<Layer*>::iterator end() { return m_layerStack.end(); }


	private:


		std::vector<Layer*> m_layerStack;


	};
}

