#include "Aozora.h"
#include <Systems/Project/Project.h>


namespace Aozora{

	void ApplicationAPI::newProject()
	{
		auto action = []() {
			auto& app = Application::getApplication();
			app.getResourceManager().clearResources();
			// TODO framebuffers must be cleared
			// TODO skybox irradiance map is never cleared
			app.m_project = std::make_unique<Aozora::Project>();
			app.m_project->setup();
			app.getRenderer().updatePrimaryScene(*app.m_project->m_currentScene);

		};
		Application::getApplication().queueAction(action);
	}

	void ApplicationAPI::loadProject()
	{
	}

	void ApplicationAPI::saveProject()
	{

		// serialization

	}

	// this doesnt replace the base scene, it just add a new one to a unordered_map of scenes.
	uint32_t ApplicationAPI::createNewScene() {
		auto& app = Aozora::Application::getApplication();
		return app.getSceneManager().createScene();
	}

	void ApplicationAPI::loadScene(uint32_t ID)
	{
		Aozora::Application::getApplication().m_project->loadScene(ID);
	}

	Scene* ApplicationAPI::getScene(uint32_t id)
	{
		auto& app = Aozora::Application::getApplication();
		return app.getSceneManager().getScene(id);
	}









}
