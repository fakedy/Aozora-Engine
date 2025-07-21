#pragma once
#include <unordered_map>
#include <Systems/Scene/Scene.h>


namespace Aozora {

	class SceneManager {
	public:


		uint32_t createScene();

		Scene* getScene(uint32_t id);


	private:

		std::unordered_map<uint32_t, Scene> m_scenes;

		int nextSceneID{ 0 };



	};
}