#pragma once
#include <Application.h> // temp
#include <Systems/EditorCameraSystem.h>
#include <memory>
#include <entt/entt.hpp>
#include <Systems/ECS/Components/Components.h>
#include <Systems/Scene/Scene.h>
#include <Systems/Project/Project.h>
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>
#include <Systems/SceneManager/SceneManager.h>
enum class EditorState { EDIT, PLAY };



class EditorLayer : public Aozora::Layer
{
public:
	EditorLayer(Aozora::SceneManager& sceneManager, Aozora::Graphics::SceneRenderer& sceneRenderer) : m_sceneManager(sceneManager){
		m_editorViewPortID = sceneRenderer.createViewport(Aozora::ViewportType::PrimaryEditor);
		m_editorCameraSystem = std::make_unique<Aozora::EditorCameraSystem>();

	}

	void onUpdate(const Aozora::Context& context) override;

	void changeState(EditorState state);

	EditorState getState();

	uint32_t m_editorViewPortID;

private:

	Aozora::SceneManager& m_sceneManager;
	EditorState m_currentState{ EditorState::EDIT };

	std::unique_ptr<Aozora::EditorCameraSystem> m_editorCameraSystem;



};

