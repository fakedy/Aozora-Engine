#include "SceneManager.h"
#include <Systems/Logging/Logger.h>
#include <random>
namespace Aozora {

	uint64_t SceneManager::createScene()
	{

		std::random_device rd;
		std::mt19937_64 gen(rd());
		std::uniform_int_distribution<uint64_t> dis;

		uint64_t genID = dis(gen);
		m_scenes[genID] = std::make_unique<Scene>();
		m_scenes[genID]->hash = genID;
		// temp stuff
		if (m_scenes.size() == 1) {
			m_activeScene = getScene(genID);
		}
		return genID; // lol
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

		auto view = scene.m_registry->view<ModelComponent>();
		for (auto& entity : view) {
			auto& model = view.get<ModelComponent>(entity);
			m_resourceManager.loadModel(model.ID, scene.hash);
		}

		// skyboxes?
		auto viewSkyboxes = scene.m_registry->view<SkyboxComponent>();
		for (auto& entity : viewSkyboxes) {
			auto& skybox = viewSkyboxes.get<SkyboxComponent>(entity);
			m_resourceManager.loadSkybox(skybox.id, scene.hash);
		}

	}
	
	void SceneManager::clearScenes()
	{
		nextSceneID = 0;
		m_scenes.clear();
		Log::info("Clearing scenes");

	}
}