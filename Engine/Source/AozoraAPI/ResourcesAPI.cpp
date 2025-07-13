#include "Aozora.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/ECS/Components/Components.h"




namespace Aozora {

	entt::entity createEntityFromNodes(Model::Node* node, entt::entity parent);

	void ResourcesAPI::loadModel(std::string path) {

		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		resourceManager.loadModel(path);


	}

	uint32_t ResourcesAPI::instantiateModel(std::string name) {

		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();


		// check if it is loaded
		if (resourceManager.m_loadedModels.find(name) == resourceManager.m_loadedModels.end()) {
			// do some error or something idk

		}

		Model& model = resourceManager.m_loadedModels.at(name);

		createEntityFromNodes(model.originNode, entt::null);

		return 0;
	}


	entt::entity createEntityFromNodes(Model::Node* node, entt::entity parent) {

		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		Scene& currentScene = Application::getApplication().getCurrentScene();
		entt::registry& registry = currentScene.getRegistry();

			const auto entity = registry.create();

			registry.emplace<Aozora::NameComponent>(entity).name = node->name.c_str();
			registry.emplace<Aozora::TagComponent>(entity);
			registry.emplace<Aozora::TransformComponent>(entity);
			if (node->hasMesh) {
				registry.emplace<Aozora::MeshComponent>(entity).meshID = node->meshID;
				resourceManager.m_loadedMeshes.at(node->meshID).bufferData();
			}
			
		Aozora::RelationComponent& relationComponent = registry.emplace<Aozora::RelationComponent>(entity, parent);

		for (Model::Node* childNode : node->childrenNodes) {

			entt::entity childEntity = createEntityFromNodes(childNode, entity);
			relationComponent.children.push_back(childEntity);
		
		}

		return entity;
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