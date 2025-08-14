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
#include <Systems/Skybox.h>

namespace Aozora {

	class ResourceManager {
	public:

		ResourceManager(Resources::AssetManager& assetManager) : m_assetManager(assetManager) {
		

		};

		const void loadModel(uint64_t hash);

		uint64_t loadTexture(uint64_t hash);
		uint64_t loadCubemap(uint64_t hash);
		uint64_t loadSkybox(uint64_t hash);
		uint64_t loadMesh(uint64_t hash);
		unsigned int createMaterial(Material* material);
		unsigned int materialLoaded(unsigned int id);
		Material& getMaterial(uint32_t id);

		// check functions
		uint64_t textureLoaded(uint64_t hash);
		bool meshLoaded(uint64_t hash);
		bool modelLoaded(uint64_t hash);

		// resources
		std::unordered_map<uint64_t, Texture> m_loadedTextures;

		// test for storing persistent vs non persistent textures
		std::unordered_map<std::string, Texture> m_loadedPersistentTextures;
		
		std::unordered_map<uint64_t, Mesh> m_loadedMeshes;
		std::unordered_map<unsigned int, Material> m_loadedmaterials;
		std::unordered_map<uint64_t, Model> m_loadedModels;
		std::unordered_map<uint64_t, Skybox> m_loadedSkyboxes;

		unsigned int m_nextMeshID{ 0 };
		unsigned int m_nextMaterialID{ 0 };

		void clearResources();

		Resources::AssetManager& m_assetManager;

	private:


	};
}
