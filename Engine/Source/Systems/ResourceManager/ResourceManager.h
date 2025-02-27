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


		const std::vector<unsigned int> loadModel(const std::string& file);

		unsigned int loadTexture(const std::string path, const std::string& directory);
		unsigned int createMaterial(Material* material);
		unsigned int materialLoaded(unsigned int id);

		// check functions
		int textureLoaded(const std::string path);
		unsigned int meshLoaded(const std::string path);

		// resources
		std::unordered_map<std::string, Material::Texture> m_loadedTextures;
		std::unordered_map<std::string, unsigned int> m_meshPathToID;
		std::unordered_map<unsigned int, Mesh> m_loadedMeshes;
		std::unordered_map<unsigned int, Material> m_loadedmaterials;

		unsigned int m_nextMeshID{ 0 };

	private:

		ResourceManager() = default;

		TextureLoader m_textureLoader = TextureLoader();
		ModelLoader m_modelLoader = ModelLoader();



	};
}
