#include "Aozora.h"
#include "Systems/ResourceManager/ResourceManager.h"





namespace Aozora {



	void ResourcesAPI::loadModel(std::string path) {

		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		resourceManager.loadModel(path);


	}

	uint32_t ResourcesAPI::instantiateModel(std::string name) {

		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();


		// check if it is loaded
		resourceManager.m_loadedModels.find(name);
		if (resourceManager.m_loadedModels.find(name) == resourceManager.m_loadedModels.end()) {
			// do some error or something idk

		}

		// dont forget to buffer the data to the gpu
		// I assume this can be problematic if we buffer duplicate data
		//createdmesh.bufferData();
		// perform the whole crazy entity creation with tree structure


		return 0;
	}


	std::vector<std::string> ResourcesAPI::getLoadedModelNames()
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		std::vector<std::string> nameVector;
		nameVector.reserve(resourceManager.m_loadedModels.size());
		for (const auto& pair : resourceManager.m_loadedModels) {
			nameVector.push_back(pair.first);
		}

		return nameVector;
	}
	Model* ResourcesAPI::getModel(std::string name)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		if (resourceManager.m_loadedModels.find(name) == resourceManager.m_loadedModels.end()) {
			// do some error idk
			return nullptr;
		}
		else {
			return &resourceManager.m_loadedModels.at(name);
		}
	}
}