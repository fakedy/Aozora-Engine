#pragma once
#include <Systems/Renderers/SceneRenderer.h>
#include <Systems/Scene/Scene.h>
#include <AozoraAPI/Aozora.h>

namespace Aozora {

	class Project {
	public:

		Project();

		std::unique_ptr<Scene> m_currentScene;
		std::unique_ptr<SceneRenderer> m_sceneRenderer;

		void setup();

	private:

	};
}
