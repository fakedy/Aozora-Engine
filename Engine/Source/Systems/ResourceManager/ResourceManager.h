#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include "Systems/Model.h"
#include "Systems/Mesh.h"
#include "Systems/Material.h"
#include <Systems/Events/Events.h>
#include <Systems/Events/EventSystem.h>
#include <Systems/AssetManager/AssetManager.h>
#include <Systems/Texture.h>

namespace Aozora {

	class ResourceManager {
	public:

		ResourceManager(Resources::AssetManager& assetManager) : m_assetManager(assetManager) {
		

		};

		const void loadModel(uint64_t hash);

		unsigned int loadTexture(const std::string fileName, const std::string& directory, bool isSrgb, bool persistent = false);
		unsigned int loadTexture(const std::string path, bool isSrgb, bool persistent = false);
		unsigned int loadCubemap(const std::vector<std::string> faces);
		unsigned int loadCubemap();
		unsigned int createMaterial(Material* material);
		unsigned int materialLoaded(unsigned int id);
		Material& getMaterial(uint32_t id);

		// check functions
		int textureLoaded(const std::string path);
		unsigned int meshLoaded(const std::string path);
		bool modelLoaded(uint64_t hash);

		// resources
		std::unordered_map<std::string, Texture> m_loadedTextures;

		// test for storing persistent vs non persistent textures
		std::unordered_map<std::string, Texture> m_loadedPersistentTextures;
		
		std::unordered_map<std::string, unsigned int> m_meshPathToID;
		std::unordered_map<unsigned int, Mesh> m_loadedMeshes;
		std::unordered_map<unsigned int, Material> m_loadedmaterials;
		std::unordered_map<uint64_t, Model> m_loadedModels;

		unsigned int m_nextMeshID{ 0 };
		unsigned int m_nextMaterialID{ 0 };

		void clearResources();

		Resources::AssetManager& m_assetManager;

	private:


	};
}
