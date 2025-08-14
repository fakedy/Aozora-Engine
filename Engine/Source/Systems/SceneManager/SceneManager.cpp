#include "SceneManager.h"
#include <Systems/Logging/Logger.h>

namespace Aozora {

	uint32_t SceneManager::createScene()
	{
		// scene id 0 should be the base scene for the project
		m_scenes[nextSceneID] = std::make_unique<Scene>();

		// temp stuff
		if (nextSceneID == 0) {
			m_activeScene = getScene(0);
		}
		nextSceneID++;
		return nextSceneID - 1; // lol
	}
	void SceneManager::deleteScene(uint32_t id)
	{
		m_scenes.erase(id);
	}
	Scene* SceneManager::getScene(uint32_t id)
	{
		auto it = m_scenes.find(id);
		if (it != m_scenes.end()) {
			return it->second.get();
		}

		Log::error("SceneManager::getScene fail on Scene doesnt exist");
		return nullptr;
	}
	Scene* SceneManager::getCurrentActiveScene()
	{
		return m_activeScene;
	}
	
	void SceneManager::clearScenes()
	{
		nextSceneID = 0;
		m_scenes.clear();
		Log::info("Clearing scenes");

	}
}