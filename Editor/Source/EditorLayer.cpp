#include "EditorLayer.h"
#include "Application.h"

void EditorLayer::onUpdate()
{

	// update systems here instead of application

	auto& app = Aozora::Application::getApplication();

	// no difference yet
	if (state == EditorState::EDIT) {

		app.m_cameraSystem->update();
		m_editorCameraSystem->update();

		app.getCurrentScene().update();

		app.getRenderer().render();
	}
	else {

		app.m_cameraSystem->update();
		m_editorCameraSystem->update();

		app.getCurrentScene().update();

		app.getRenderer().render();

	}



}
