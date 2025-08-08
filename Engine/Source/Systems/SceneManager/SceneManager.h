#pragma once
#include <unordered_map>
#include <Systems/Scene/Scene.h>
#include <Systems/Events/Events.h>
#include <Systems/Events/EventSystem.h>


/*
* The role of the scenemanager is to hold active scenes
* the getCurrentActivescene() doesnt actually make sense since we'll probably have several active scenes,
* an example would be like in unreal engine where you can create a blueprint
* It's also convenient for any viewport rendering with assets like a material or model viewer.
*/

namespace Aozora {

	class SceneManager {
	public:

		SceneManager() {

		}

		uint32_t createScene();
		void deleteScene(uint32_t id);

		Scene* getScene(uint32_t id);

		Scene* getCurrentActiveScene();

		void clearScenes();

	private:

		std::unordered_map<uint32_t, std::unique_ptr<Scene>> m_scenes;

		int nextSceneID{ 0 };

		Scene* m_activeScene = nullptr;



	};
}