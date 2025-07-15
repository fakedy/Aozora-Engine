#pragma once
#include "AozoraAPI/Aozora.h"
#include <Systems/EditorCameraSystem.h>
#include <memory>
#include <entt/entt.hpp>
#include <Systems/ECS/Components/Components.h>
#include <Systems/Scene/Scene.h>




enum class EditorState { EDIT, PLAY };



class EditorLayer : public Aozora::Layer
{
public:
	EditorLayer() {

		Aozora::Scene& scene = Aozora::Application::getApplication().getCurrentScene();
		m_editorCameraSystem = std::make_unique<Aozora::EditorCameraSystem>();
		// maybe dependency injection isnt that bad
		entt::registry& registry = Aozora::Application::getApplication().getCurrentScene().getRegistry();

		// creating the editor camera
		// will be invisible to the scene graph
		const entt::entity editorCameraEntity = registry.create();
		transformComponent = &registry.emplace<Aozora::TransformComponent>(editorCameraEntity);
		cameraComponent = &registry.emplace<Aozora::CameraComponent>(editorCameraEntity);
		registry.emplace<Aozora::EditorEntityTag>(editorCameraEntity);

		m_editorViewPortID = Aozora::RenderAPI::createViewport(&scene, Aozora::ViewportType::PrimaryEditor, editorCameraEntity);
		m_gameViewPortID = Aozora::RenderAPI::createViewport(&scene, Aozora::ViewportType::PrimaryGame);

		// temp loading models
		Aozora::ResourcesAPI::loadModel("Resources/testcube/testcube.obj");
		Aozora::ResourcesAPI::loadModel("Resources/sponza2/sponza.obj");
		Aozora::ResourcesAPI::loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//Aozora::ResourcesAPI::loadModel("Resources/San_Miguel/san-miguel.obj");
		Aozora::ResourcesAPI::loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
		Aozora::ResourcesAPI::loadModel("Resources/gpmesh/scene.gltf");
		//Aozora::ResourcesAPI::loadModel("Resources/living_room/living_room.obj");
		//Aozora::ResourcesAPI::loadModel("Resources/sibenik/sibenik.obj");
		Aozora::ResourcesAPI::loadModel("Resources/conference/conference.obj");
	}

	void onUpdate() override;

	uint32_t m_gameViewPortID;
	uint32_t m_editorViewPortID;

private:

	Aozora::CameraComponent* cameraComponent;
	Aozora::TransformComponent* transformComponent;
	EditorState state{ EditorState::EDIT };

	std::unique_ptr<Aozora::EditorCameraSystem> m_editorCameraSystem;


	// temp stuff

	float lastX = 1920 / 2.0f;
	float lastY = 1080 / 2.0f;

	float yaw = -90.0f; // rotate
	float pitch = 0.0f; // up and down

	float sensitivity = 0.3f;


	float movspeed{ 10.0f };

	bool mouseHeld{ false };

};

