#include <AozoraAPI/Aozora.h>
#include <Systems/Scene/Scene.h>
#include <Application.h>

#include "Systems/ECS/Components/Components.h"

namespace Aozora {

	void SceneAPI::addEntity() {


		Scene& currentScene = Application::getApplication().getCurrentScene();
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		entt::registry& registry = currentScene.getRegistry();
		const auto entity = registry.create();

		// this will be way more complicated now pog 
		// one entity per mesh
		registry.emplace<Aozora::NameComponent>(entity).name = "Entity";
		registry.emplace<Aozora::TagComponent>(entity);
		registry.emplace<Aozora::TransformComponent>(entity);
		registry.emplace<Aozora::RelationComponent>(entity);


		//registry.emplace<Aozora::MeshComponent>(entity).meshIDs = resourceManager.loadModel("Resources/testcube/testcube.obj");


		//m_resourceManager.loadModel("Resources/cube/cube.obj");
		//m_resourceManager.loadModel("Resources/cube/cube.obj");
		//m_meshes = source.loadModel("Resources/sponza/sponza.obj");
		//m_meshes = source.loadModel("Resources/cube/cube.obj");
		//m_meshes = source.loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//m_meshes = source.loadModel("Resources/hintze-hall-vr-tour/source/hintze-hall_UV_pack01.fbx");
		//m_meshes = source.loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
		//m_meshes = source.loadModel("Resources/cube/cube.obj");
		//m_resourceManager.loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//m_meshes = source.loadModel("Resources/gpmesh/scene.gltf");

		/*
		std::cout << "Material count: " << resourceManager.m_loadedmaterials.size() << "\n";
		std::cout << "Texture count: " << resourceManager.m_loadedTextures.size() << "\n";
		std::cout << "Mesh count: " << resourceManager.m_loadedMeshes.size() << "\n";
		*/
	}

	void SceneAPI::deleteEntity() {




	}

	std::vector<entt::entity> SceneAPI::getSceneHierarchyEntities()
	{

		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();

		// everything but editor related entities
		// want to sort this by name first
		auto view = registry.view<NameComponent>(entt::exclude<EditorEntityTag>);

		return { view.begin(), view.end() };
	}

	std::string SceneAPI::getEntityName(entt::entity entity)
	{
		entt::registry& registry = Application::getApplication().getCurrentScene().getRegistry();
		auto target = registry.get<NameComponent>(entity);
		return target.name;
	}











}
