#include "ProjectManager.h"
#include <iostream>
#include <Systems/Logging/Logger.h>

namespace Aozora {

	bool ProjectManager::createProject(std::string name)
	{
		// check if it exist blablabla
		m_project = std::make_unique<Project>();

		Log::info("Created new project");
		return true;
	}
}
