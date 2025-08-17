#pragma once
#include <unordered_map>
#include <memory>
#include <iostream>
#include <glad/glad.h>
#include <Systems/AssetManager/AssetManager.h>
#include "Systems/Model.h"
#include "Systems/Mesh.h"
#include "Systems/Material.h"
#include <Systems/Texture.h>
#include <Systems/Skybox.h>
#include <Systems/Renderers/OpenGL.h>
#include <Systems/Renderers/IrenderAPI.h>

namespace Aozora {

	class ResourceManager {
	public:

		ResourceManager(Resources::AssetManager& assetManager, IrenderAPI& renderAPI) : m_assetManager(assetManager), m_renderAPI(renderAPI) {
		

		};

		struct ResourceContainer {
			std::unordered_map<uint64_t, Texture> m_loadedTextures;
			std::unordered_map<uint64_t, Mesh> m_loadedMeshes;
			std::unordered_map<uint64_t, Material> m_loadedmaterials;
			std::unordered_map<uint64_t, Model> m_loadedModels;
			std::unordered_map<uint64_t, Skybox> m_loadedSkyboxes;
			unsigned int m_nextMeshID{ 0 };
			unsigned int m_nextMaterialID{ 0 };
		};

		const void loadModel(uint64_t hash, uint64_t sceneID);

		uint64_t loadTexture(uint64_t hash, uint64_t sceneID);
		uint64_t loadTexturePersistent(uint64_t hash);
		uint64_t loadMaterial(uint64_t hash, uint64_t sceneID);
		uint64_t loadCubemap(uint64_t hash, uint64_t sceneID);
		
		uint64_t createEmptyCubeMap(uint32_t width, uint32_t height, uint64_t sceneID);
		uint64_t loadSkybox(uint64_t hash, uint64_t sceneID);
		uint64_t loadMesh(uint64_t hash, uint64_t sceneID);
		uint64_t createMaterial(Material* material);
		uint64_t materialLoaded(uint64_t id, uint64_t sceneID);
		Material& getMaterial(uint64_t id, uint64_t sceneID);

		// check functions
		uint64_t textureLoaded(uint64_t hash, uint64_t sceneID);
		uint64_t meshLoaded(uint64_t hash, uint64_t sceneID);
		uint64_t modelLoaded(uint64_t hash, uint64_t sceneID);

		// test for storing persistent vs non persistent textures
		std::unordered_map<uint64_t, Texture> m_loadedPersistentTextures;

		
		std::unordered_map<uint64_t, ResourceContainer> m_containerMap;

		void clearResources(uint64_t sceneID);

		Resources::AssetManager& m_assetManager;
		IrenderAPI& m_renderAPI;

	private:


	};
}
