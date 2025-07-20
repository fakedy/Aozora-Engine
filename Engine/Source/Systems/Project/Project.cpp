#include "Project.h"

Aozora::Project::Project()
{

	m_currentScene = std::make_unique<Scene>();
	m_sceneRenderer = std::make_unique<SceneRenderer>();
	m_sceneRenderer->updatePrimaryScene(m_currentScene.get());

}

void Aozora::Project::setup()
{
	// temp loading models

	ResourcesAPI::loadModel("Resources/testcube/testcube.obj");
	ResourcesAPI::loadModel("Resources/sponza2/sponza.obj");
	ResourcesAPI::loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
	//Aozora::ResourcesAPI::loadModel("Resources/San_Miguel/san-miguel.obj");
	ResourcesAPI::loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
	ResourcesAPI::loadModel("Resources/gpmesh/scene.gltf");
	//Aozora::ResourcesAPI::loadModel("Resources/living_room/living_room.obj");
	//Aozora::ResourcesAPI::loadModel("Resources/sibenik/sibenik.obj");
	ResourcesAPI::loadModel("Resources/conference/conference.obj");

}
