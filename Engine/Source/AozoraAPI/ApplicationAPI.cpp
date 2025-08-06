#include "Aozora.h"
#include <Systems/Project/Project.h>


namespace Aozora{


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
