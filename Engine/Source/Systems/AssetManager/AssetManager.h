#pragma once
#include <filesystem>
#include <unordered_map>

namespace Aozora::Resources {

	class AssetManager {
	public:


		AssetManager();
		enum class Type {
			Scene,
			Model,
			Texture,
			Material
		};
		struct Asset {
			Type type;
			std::string parentDir{"Assets"}; // default to Assets
			std::string name;
			uint32_t icon; // can hold an ID to a texture for possible custom icon
		};

		// GOALS -----------
		// load from disk here, store models in binary format.
		// create virtual filesystem for the editor
		// resourcemanager will ask for stuff to be loaded from disk
		// no file will ever be loaded without passing through here
		// I just cant decide how I want to store the hierarchy relation


		bool createProject(std::string name);
		void createScene();

		Asset& getAsset(uint32_t ID);

	private:

		std::unordered_map<uint32_t, Asset> m_assets; // for fast asset access


	};
}
