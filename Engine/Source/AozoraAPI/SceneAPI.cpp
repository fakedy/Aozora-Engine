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

		registry.emplace<Aozora::NameComponent>(entity).name = "Entity";
		registry.emplace<Aozora::TagComponent>(entity);
		registry.emplace<Aozora::TransformComponent>(entity);
		registry.emplace<Aozora::RelationComponent>(entity);
		registry.emplace<Aozora::MeshComponent>(entity).meshIDs = resourceManager.loadModel("Resources/cube/cube.obj");


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

		std::cout << "Material count: " << resourceManager.m_loadedmaterials.size() << "\n";
		std::cout << "Texture count: " << resourceManager.m_loadedTextures.size() << "\n";
		std::cout << "Mesh count: " << resourceManager.m_loadedMeshes.size() << "\n";
	}

	void SceneAPI::deleteEntity() {




	}











}
