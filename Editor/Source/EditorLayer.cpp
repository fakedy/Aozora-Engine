#include "EditorLayer.h"
#include "Application.h"
#include "Systems/Input.h"
#include "glm/glm.hpp"
#include <Systems/Time.h>

void EditorLayer::onUpdate()
{

	// update systems here instead of application

	auto& app = Aozora::Application::getApplication();

	// no difference yet
	if (state == EditorState::EDIT) {

		app.m_cameraSystem->update(app.m_project->m_currentScene->getRegistry());
		m_editorCameraSystem->update(app.m_project->m_currentScene->getRegistry());

		app.getCurrentScene().update();

		app.getRenderer().render();
	}
	else {

		app.m_cameraSystem->update(app.m_project->m_currentScene->getRegistry());
		m_editorCameraSystem->update(app.m_project->m_currentScene->getRegistry());

		app.getCurrentScene().update();

		app.getRenderer().render();

	}



}
