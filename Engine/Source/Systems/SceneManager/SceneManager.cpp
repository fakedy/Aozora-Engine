#include "SceneManager.h"
#include <Systems/Logging/Logger.h>

namespace Aozora {

	uint64_t SceneManager::createScene()
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
	void SceneManager::deleteScene(uint64_t id)
	{
		m_scenes.erase(id);
	}
	Scene* SceneManager::getScene(uint64_t id)
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

	void SceneManager::loadScene(const Scene& scene)
	{

		// need to load in the required stuff into ram using the IDS
		// Maybe i should serialize the resourcemanager 


		// make sure its not the same scene
		if (scene.hash == m_activeScene->hash) {
			m_activeScene->loadSnapShot();
			return;
		}
		
		// add the new scene to the unordered map
		m_scenes[scene.hash] = std::make_unique<Scene>(scene);
		// load the saved scene data
		m_scenes[scene.hash]->loadSnapShot();
		deleteScene(m_activeScene->hash);
		m_activeScene = m_scenes[scene.hash].get();
	}
	
	void SceneManager::clearScenes()
	{
		nextSceneID = 0;
		m_scenes.clear();
		Log::info("Clearing scenes");

	}
}