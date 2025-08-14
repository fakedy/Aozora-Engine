#pragma once
#include <filesystem>
#include <unordered_map>
#include "ModelLoader.h"
#include <functional>
#include <Systems/AssetManager/TextureLoader.h>
#include <cereal/cereal.hpp>
#include <cereal/types/unordered_map.hpp>
#include <Systems/Skybox.h>

namespace Aozora::Resources {

		enum class AssetType {
			Scene,
			Model,
			Texture,
			Material,
			Mesh,
			Skybox
		};
		struct Asset {
			AssetType type;
			std::string parentDir{"Assets"}; // default to Assets
			std::string name;
			uint32_t icon; // can hold an ID to a texture for possible custom icon
			uint32_t hash;
			bool hidden{ false };
		};

	class AssetManager {
	public:


		AssetManager();

		// GOALS -----------
		// load from disk here, store models in binary format.
		// create virtual filesystem for the editor
		// resourcemanager will ask for stuff to be loaded from disk
		// no file will ever be loaded without passing through here
		// I just cant decide how I want to store the hierarchy relation


		bool createProject(std::string name);

		Asset& getAsset(uint32_t ID);

		void loadAsset(const std::string& path);

		Model loadModelFromDisk(uint64_t hash);
		Mesh loadMeshFromDisk(uint64_t hash);
		Texture loadTextureFromDisk(uint64_t hash);
		Material loadMaterialFromDisk(uint64_t hash);
		Skybox loadSkyboxFromDisk(uint64_t hash);

		uint64_t createSkybox();

		uint64_t getUniqueID();

		std::vector<std::reference_wrapper<Asset>> getLoadedAssets();

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(m_importRegistry));
		}

	private:

		std::unordered_map<uint64_t, Asset> m_assets; // for fast asset access
		std::unordered_map<std::string, uint64_t> m_importRegistry;
		uint32_t m_nextAssetID{ 0 };

		TextureLoader m_textureLoader = TextureLoader(m_importRegistry);
		ModelLoader m_modelLoader = ModelLoader(m_importRegistry, m_textureLoader);

		std::string m_workingDirectory{};
		std::string m_projectDir{};

	};
}
