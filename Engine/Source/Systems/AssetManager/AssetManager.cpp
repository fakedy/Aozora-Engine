#include "AssetManager.h"
#include <iostream>

namespace Aozora::Resources {

	AssetManager::AssetManager()
	{



	}

	bool AssetManager::createProject(std::string name)
	{
		// use serializer class instead for most things but whatever
		std::filesystem::path projectPath = "Editor/Projects/" + name;
		if (std::filesystem::create_directory(projectPath)) {
			std::cout << "Successfully created project" << std::endl;
			return true;
		}
		else {
			std::cout << "Name already take";
			return false;
		}
	}

	void AssetManager::createScene()
	{


	}


}

