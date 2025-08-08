#include "AssetManager.h"
#include <iostream>
#include <Systems/Logging/Logger.h>
namespace Aozora::Resources {

	AssetManager::AssetManager()
	{
		


	}

	bool AssetManager::createProject(std::string name)
	{
		// use serializer class instead for most things but whatever
		std::filesystem::path assetsPath = "Projects/" + name + "/Assets";
		if (std::filesystem::create_directories(assetsPath)) {
			Log::info("Successfully created project folder");

			// load base assets and serialize to store binary data


			return true;
		}





		Log::error("Failed to create project directory. It may already exist.");
		return false;
	}

	void AssetManager::createScene()
	{


	}

	AssetManager::Asset& AssetManager::getAsset(uint32_t ID)
	{
		if (m_assets.find(ID) != m_assets.end()) {
			return m_assets[ID];
		}
		throw std::out_of_range(std::format("Asset with ID: {} doesn't exist", ID));
	}


}

