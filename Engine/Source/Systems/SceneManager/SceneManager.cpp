#include "SceneManager.h"

namespace Aozora {

	uint32_t SceneManager::createScene()
	{
		// scene id 0 should be the base scene for the project
		m_scenes[nextSceneID] = std::make_unique<Scene>();
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
		std::cout << "Scene doesnt exist\n";
		return nullptr;
	}
}