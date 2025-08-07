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
		std::filesystem::path projectPath = "Projects/" + name;
		if (std::filesystem::create_directories(projectPath)) {
			Log::info("Successfully created project folder");
			return true;
		}
		else {
			Log::error("Failed to create project directory. It may already exist.");
			return false;
		}
	}

	void AssetManager::createScene()
	{


	}


}

