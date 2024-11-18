#pragma once
#include "ModelLoader.h"


namespace Aozora {

	class ResourceManager {
	public:

		ResourceManager();


		const std::vector<Mesh> loadModel(const std::string& file);

	private:

		static ResourceManager* m_resourceManager;

		ModelLoader* m_modelLoader;

	};
}
