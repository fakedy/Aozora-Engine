#include "EditorLayer.h"
#include "Application.h"
#include "Systems/Input.h"
#include "glm/glm.hpp"
#include <Systems/Time.h>

void EditorLayer::onUpdate(const Aozora::Context& context)
{
	auto scene = *context.sceneManager->getCurrentActiveScene();
	auto& registry = scene.getRegistry();
	auto renderer = *context.sceneRenderer;
	auto scriptSystem = *context.scriptSystem;
	auto cameraSystem = *context.cameraSystem;
	EditorUpdateParams params{
		scene,
		registry,
		renderer,
		scriptSystem,
		cameraSystem
	};
	internalUpdate(params);
}

void EditorLayer::internalUpdate(const EditorUpdateParams& params) {
	// update systems here instead of application

	// We dont swap to the game camera during Game mode which make the viewport freeze during Game.
	if (m_currentState == EditorState::EDIT) {

		params.cameraSystem.update(params.registry);


		m_editorCameraSystem->update(params.registry);
	

		params.scene.update();

		params.renderer.render();
	}
	else {

		params.cameraSystem.update(params.registry);
		params.scriptSystem.update(params.registry);
		params.scene.update();

		params.renderer.render();

	}
}

void EditorLayer::changeState(EditorState state)
{
	auto& app = Aozora::Application::getApplication();

	switch (state)
	{
	case EditorState::EDIT:
		app.getSceneManager().getCurrentActiveScene()->loadSnapShot();
		break;
	case EditorState::PLAY:
		app.getSceneManager().getCurrentActiveScene()->takeSnapshot();

		break;
	default:
		break;
	}
	m_currentState = state;
	
}

void EditorLayer::onAttach() {

}

EditorState EditorLayer::getState()
{
	return m_currentState;
}

