#include "Aozora.h"
#include <Systems/Project/Project.h>


namespace Aozora{

	void ApplicationAPI::newProject()
	{
		auto action = []() {
			auto& app = Application::getApplication();
			app.getResourceManager().clearResources();
			app.m_project = std::make_unique<Aozora::Project>();
			app.m_project->setup();
			app.getRenderer().updatePrimaryScene(*app.m_project->m_currentScene);

		};
		Application::getApplication().queueAction(action);
		// surely there is a gpu memory leak?
		// i dont clear the models and textures that are sitting on the gpu
	}

	void ApplicationAPI::loadProject()
	{
	}

	void ApplicationAPI::saveProject()
	{
	}

	// this doesnt replace the base scene, it just add a new one to a unordered_map of scenes.
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
