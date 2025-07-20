#include "Aozora.h"









namespace Aozora{





	void ApplicationAPI::newProject()
	{

		createNewScene();
	}

	void ApplicationAPI::loadProject()
	{
	}

	void ApplicationAPI::saveProject()
	{
	}

	void ApplicationAPI::createNewScene() {

		auto action = []() {
		};

		Application::getApplication().queueAction(action);
	}

	void ApplicationAPI::loadScene()
	{
	}









}
