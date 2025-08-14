#pragma once 
#include <string>
#include <memory>
#include "Project.h"
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/Events/Events.h>
#include <Systems/Events/EventSystem.h>

namespace Aozora {

	class ProjectManager {
	public:

		ProjectManager(SceneManager& sceneManager) : m_sceneManager(sceneManager) {

		}



		bool createProject(std::string name);

	private:
		std::unique_ptr<Project> m_project;
		SceneManager& m_sceneManager;
	};
}