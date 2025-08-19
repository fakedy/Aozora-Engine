#pragma once
#include <unordered_map>
#include <Systems/Scene/Scene.h>
#include <memory>
#include <Systems/ResourceManager/ResourceManager.h>

/*
* The role of the scenemanager is to hold active scenes
* the getCurrentActivescene() doesnt actually make sense since we'll probably have several active scenes,
* an example would be like in unreal engine where you can create a blueprint
* It's also convenient for any viewport rendering with assets like a material or model viewer.
*/

namespace Aozora {

	class SceneManager {
	public:

		SceneManager(ResourceManager& resourceManager) : m_resourceManager(resourceManager) {

		}

		uint64_t createScene();
		void deleteScene(uint64_t id);

		Scene* getScene(uint64_t id);

		Scene* getCurrentActiveScene();

		void loadScene(const Scene& scene);

		void clearScenes();

	private:

		std::unordered_map<uint64_t, std::unique_ptr<Scene>> m_scenes;

		int nextSceneID{ 0 };

		Scene* m_activeScene = nullptr;
		ResourceManager& m_resourceManager;



	};
}