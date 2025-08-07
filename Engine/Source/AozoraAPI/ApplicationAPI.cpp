#include "Aozora.h"
#include <Systems/Project/Project.h>


namespace Aozora{


	Scene* ApplicationAPI::getScene(uint32_t id)
	{
		auto& app = Aozora::Application::getApplication();
		return app.getSceneManager().getScene(id);
	}


}
