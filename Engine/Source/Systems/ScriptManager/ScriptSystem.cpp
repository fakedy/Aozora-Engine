#include "ScriptSystem.h"
#include "Systems/ECS/Components/ScriptComponent.h"


namespace Aozora {


	void ScriptSystem::update(entt::registry& registry)
	{

		// use entt to get all script components




		auto view = registry.view<ScriptComponent>();


		for (auto& script : view) {
			auto& scriptComponents = view.get<ScriptComponent>(script);
		
			// components contain an ID key for the unordered map
			// if the map does not contain the script ID, we create a new script instance
			if (m_scriptInstances.find(scriptComponents.scriptID) == m_scriptInstances.end()) {
				
				// LINKER ERROR
				/*
				// create a new script instance
				ScriptInstance instance;
				instance.thread = lua_newthread(L);
				m_scriptInstances[scriptComponents.scriptID] = instance; // populate map with instance
				std::cout << "Script instance created for script ID: " << scriptComponents.scriptID << std::endl;
				*/
			}



		}



	}
}

