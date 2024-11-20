#pragma once
#include "ModelLoader.h"
#include "TextureLoader.h"


namespace Aozora {

	class ResourceManager {
	public:


		
		static ResourceManager& getResourceManager() {
			static ResourceManager instance;
			return instance;
		}

		ResourceManager(ResourceManager const&) = delete;
		void operator =(ResourceManager const&) = delete;


		const std::vector<Mesh> loadModel(const std::string& file);

		unsigned int loadTexture(const char* path);

	private:

		ResourceManager() = default;

		TextureLoader m_textureLoader = TextureLoader();
		ModelLoader m_modelLoader = ModelLoader();

	};
}
