#include "Project.h"
#include <AozoraAPI/Aozora.h>


namespace Aozora {

	Project::Project()
	{

	}

	void Project::setup()
	{
		// temp loading models
		newScene();



		ResourcesAPI::loadModel("Resources/testcube/testcube.obj");
		//ResourcesAPI::loadModel("Resources/sponza2/sponza.obj");
		//ResourcesAPI::loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//Aozora::ResourcesAPI::loadModel("Resources/San_Miguel/san-miguel.obj");
		//ResourcesAPI::loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
		ResourcesAPI::loadModel("Resources/gpmesh/scene.gltf");
		//Aozora::ResourcesAPI::loadModel("Resources/living_room/living_room.obj");
		//Aozora::ResourcesAPI::loadModel("Resources/sibenik/sibenik.obj");
		//ResourcesAPI::loadModel("Resources/conference/conference.obj");

		entt::registry& registry = m_currentScene->getRegistry();

		Aozora::CameraComponent* cameraComponent;
		Aozora::TransformComponent* transformComponent;

		// creating the editor camera
		// will be invisible to the scene graph
		const entt::entity editorCameraEntity = registry.create();
		transformComponent = &registry.emplace<Aozora::TransformComponent>(editorCameraEntity);
		cameraComponent = &registry.emplace<Aozora::CameraComponent>(editorCameraEntity);
		registry.emplace<Aozora::EditorEntityTag>(editorCameraEntity);


	}

	void Project::newScene()
	{
		uint32_t sceneID = ApplicationAPI::createNewScene();
		m_currentScene = ApplicationAPI::getScene(sceneID);
	}
}
