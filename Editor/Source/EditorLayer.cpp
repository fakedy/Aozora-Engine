#include "EditorLayer.h"
#include "Application.h"
#include "Systems/Input.h"
#include "glm/glm.hpp"
#include <Systems/Time.h>

void EditorLayer::onUpdate(const Aozora::Context& context)
{
	Aozora::Scene* scene = context.sceneManager->getCurrentActiveScene();
	auto& registry = scene->getRegistry();
	EditorUpdateParams params{
		*scene,
		registry,
		* context.sceneRenderer,
		* context.scriptSystem,
		* context.cameraSystem
	};
	internalUpdate(params);
}

void EditorLayer::internalUpdate(const EditorUpdateParams& params) {
	// update systems here instead of application

	// We dont swap to the game camera during Game mode which make the viewport freeze during Game.
	if (m_currentState == EditorState::EDIT) {
		m_editorCameraSystem->update(params.registry);
		params.renderer.getViewport(m_editorViewPortID).type = Aozora::ViewportType::PrimaryEditor;
	}
	else {
		params.renderer.getViewport(m_editorViewPortID).type = Aozora::ViewportType::PrimaryGame;
		params.cameraSystem.update(params.registry);
		params.scriptSystem.update(params.registry);
	}
	params.scene.update();
	params.renderer.render();
}

void EditorLayer::changeState(EditorState state)
{
	switch (state)
	{
	case EditorState::EDIT:
		m_sceneManager.getCurrentActiveScene()->loadSnapShot();
		break;
	case EditorState::PLAY:
		m_sceneManager.getCurrentActiveScene()->takeSnapshot();
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

