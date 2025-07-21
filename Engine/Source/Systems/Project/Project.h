#pragma once
#include <Systems/Renderers/SceneRenderer.h>
#include <Systems/Scene/Scene.h>
#include <memory>

namespace Aozora {

	class Project {
	public:

		Project();

		Scene* m_currentScene;

		void setup();

		void newScene();

	private:


		uint32_t sceneID;
	};
}
