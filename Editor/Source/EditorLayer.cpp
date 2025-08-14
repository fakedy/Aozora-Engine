#include "EditorLayer.h"
#include "Application.h"
#include "Systems/Input.h"
#include "glm/glm.hpp"
#include <Systems/Time.h>

void EditorLayer::onUpdate(const Aozora::Context& context)
{


	// update systems here instead of application

	auto& app = Aozora::Application::getApplication();
	auto current_scene = app.getSceneManager().getCurrentActiveScene();

	// We dont swap to the game camera during Game mode which make the viewport freeze during Game.
	if (m_currentState == EditorState::EDIT) {

		app.m_cameraSystem->update(current_scene->getRegistry());
		m_editorCameraSystem->update(current_scene->getRegistry());

		app.getSceneManager().getCurrentActiveScene()->update();

		app.getRenderer().render();
	}
	else {

		app.m_cameraSystem->update(current_scene->getRegistry());

		app.getScriptSystem().update(current_scene->getRegistry());
		app.getSceneManager().getCurrentActiveScene()->update();

		app.getRenderer().render();

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

EditorState EditorLayer::getState()
{
	return m_currentState;
}

