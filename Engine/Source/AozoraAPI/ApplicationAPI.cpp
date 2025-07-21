#include "Aozora.h"
#include <Systems/Project/Project.h>


namespace Aozora{

	void ApplicationAPI::newProject()
	{
		auto action = []() {
			auto& app = Application::getApplication();
			app.m_project = std::make_unique<Aozora::Project>();
			app.m_project->setup();
			app.getRenderer().updatePrimaryScene(*app.m_project->m_currentScene);
			// doesnt even work
			/*
			ChangeSceneEvent event(*app.m_project->m_currentScene);
			EventDispatcher::dispatch(event);7
			*/
		};
		Application::getApplication().queueAction(action);
	}

	void ApplicationAPI::loadProject()
	{
	}

	void ApplicationAPI::saveProject()
	{
	}

	uint32_t ApplicationAPI::createNewScene() {
		auto& app = Aozora::Application::getApplication();
		return app.getSceneManager().createScene();
	}

	void ApplicationAPI::loadScene()
	{
	}

	Scene* ApplicationAPI::getScene(uint32_t id)
	{
		auto& app = Aozora::Application::getApplication();
		return app.getSceneManager().getScene(id);
	}









}
