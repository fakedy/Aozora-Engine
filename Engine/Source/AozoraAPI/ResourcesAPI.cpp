#include "Aozora.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/ECS/Components/Components.h"
#include "Systems/Renderers/IrenderAPI.h"




namespace Aozora {

	entt::entity createEntityFromNodes(Model::Node* node, entt::entity parent);

	void ResourcesAPI::loadModel(std::string path) {

		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		resourceManager.loadModel(path);


	}

	uint32_t ResourcesAPI::loadCubemap(const std::vector<std::string>& paths)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		return resourceManager.loadCubemap(paths);
	}
	uint32_t ResourcesAPI::loadCubemap()
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		return resourceManager.loadCubemap();
	}

	SkyboxTextures ResourcesAPI::loadSkybox(const std::vector<std::string>& paths)
	{
		SkyboxTextures data;
		data.skyboxTextureID = loadCubemap(paths);
		// bake the irradiance map
		IrenderAPI& renderAPI = Application::getApplication().getRenderAPI();

		data.irradianceTextureID = renderAPI.bakeCubemapIrradiance(data.skyboxTextureID, loadCubemap());
		return data;
	}

	uint32_t ResourcesAPI::instantiateModel(std::string name) {

		ResourceManager& resourceManager = Application::getApplication().getResourceManager();


		// check if it is loaded
		if (resourceManager.m_loadedModels.find(name) == resourceManager.m_loadedModels.end()) {
			// do some error or something idk

		}

		Model& model = resourceManager.m_loadedModels.at(name);

		createEntityFromNodes(model.originNode, entt::null);

		return 0;
	}

	uint32_t ResourcesAPI::loadTexture(std::string name, bool persistent)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		return resourceManager.loadTexture(name, false, persistent);
	}

	// with the nodes we create the actually entities
	entt::entity createEntityFromNodes(Model::Node* node, entt::entity parent) {

		ResourceManager& resourceManager = Application::getApplication().getResourceManager();
		Scene& currentScene = Application::getApplication().getCurrentScene();
		entt::registry& registry = currentScene.getRegistry();

			const auto entity = registry.create();

			registry.emplace<Aozora::NameComponent>(entity).name = node->name.c_str();
			registry.emplace<Aozora::TagComponent>(entity);
			registry.emplace<Aozora::TransformComponent>(entity);
			if (node->hasMesh) {
				// add a mesh component to the entity with the id of the mesh
				auto& meshComp = registry.emplace<Aozora::MeshComponent>(entity);
				meshComp.meshID = node->meshID;
				meshComp.materialID = resourceManager.m_loadedMeshes.at(node->meshID).materialID;
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
	Material& ResourcesAPI::getMaterial(uint32_t id)
	{
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		return resourceManager.m_loadedmaterials[id];
	}
}