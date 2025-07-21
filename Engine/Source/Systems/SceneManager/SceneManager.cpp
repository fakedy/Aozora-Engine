#include "SceneManager.h"

namespace Aozora {

	uint32_t SceneManager::createScene()
	{
		// scene id 0 should be the base scene for the project
		Scene newScene;
		m_scenes[nextSceneID] = newScene;
		nextSceneID++;
		return nextSceneID - 1; // lol
	}
	Scene* SceneManager::getScene(uint32_t id)
	{
		auto it = m_scenes.find(id);
		if (it != m_scenes.end()) {
			return &it->second;
		}
		std::cout << "Scene doesnt exist\n";
		return nullptr;
	}
}
