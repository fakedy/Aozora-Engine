#pragma once
#include "ModelLoader.h"
#include "TextureLoader.h"
#include "unordered_map"
#include <memory>
#include <iostream>
#include <glad/glad.h>


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

		unsigned int loadTexture(const std::string path, const std::string& directory);


	private:

		ResourceManager() = default;

		TextureLoader m_textureLoader = TextureLoader();
		ModelLoader m_modelLoader = ModelLoader();

		std::unordered_map<std::string, Material::Texture> m_loadedTextures;

		int textureLoaded(const std::string path);


	};
}
