#pragma once
#include "ModelLoader.h"
#include "unordered_map"
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include "Systems/Model.h"


namespace Aozora {

	class ResourceManager {
	public:

		ResourceManager() {};

		const void loadModel(const std::string& file);

		unsigned int loadTexture(const std::string fileName, const std::string& directory, bool isSrgb, bool persistent = false);
		unsigned int loadTexture(const std::string path, bool isSrgb, bool persistent = false);
		unsigned int loadCubemap(const std::vector<std::string> faces);
		unsigned int loadCubemap();
		unsigned int createMaterial(Material* material);
		unsigned int materialLoaded(unsigned int id);

		// check functions
		int textureLoaded(const std::string path);
		unsigned int meshLoaded(const std::string path);
		bool modelLoaded(const std::string path);

		void clearResources();
		// resources
		std::unordered_map<std::string, Material::Texture> m_loadedTextures;

		// test for storing persistent vs non persistent textures
		std::unordered_map<std::string, Material::Texture> m_loadedPersistentTextures;
		
		std::unordered_map<std::string, unsigned int> m_meshPathToID;
		std::unordered_map<unsigned int, Mesh> m_loadedMeshes;
		std::unordered_map<unsigned int, Material> m_loadedmaterials;
		std::unordered_map<std::string, Model> m_loadedModels;

		unsigned int m_nextMeshID{ 0 };
		unsigned int m_nextMaterialID{ 0 };


	private:

		ModelLoader m_modelLoader = ModelLoader();



	};
}
