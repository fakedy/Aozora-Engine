#pragma once
// Includes used by applications

#include "Application.h"
#include "Systems/Layers/Layer.h"
#include "Systems/Renderers/Viewport.h"
#include "entt/entt.hpp"
#include <string>
#include <Systems/Model.h>
#include <Systems/Mesh.h>
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>

namespace Aozora {


	// api
	// GONNA EXECUTE THIS ENTIRE API, HATE IT


	class RenderAPI {
	public:

		static uint32_t createViewport(ViewportType type, entt::entity editorCameraEntity);
		static uint32_t createViewport(ViewportType type);


	};

	class ApplicationAPI {
	public:
		static Scene* getScene(uint32_t id);


	};

	class ResourcesAPI {
	public:

		static void loadModel(std::string path);
		static uint32_t loadCubemap(const std::vector<std::string>& paths);
		static uint32_t loadCubemap();
		static SkyboxTextures loadSkybox(const std::vector<std::string>& paths);
		static uint32_t loadTexture(std::string name, bool persistent);

		static std::vector<std::string> getLoadedModelNames();
		static Model* getModel(std::string name);

		// cant remove this yet i need to fix my rendering pipeline
		static Material& getMaterial(uint32_t id);

	};

}
