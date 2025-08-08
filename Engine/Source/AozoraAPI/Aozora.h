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




	class ResourcesAPI {
	public:

		static uint32_t loadCubemap(const std::vector<std::string>& paths);
		static uint32_t loadCubemap();
		static SkyboxTextures loadSkybox(const std::vector<std::string>& paths);
		static uint32_t loadTexture(std::string name, bool persistent);

		static std::vector<std::string> getLoadedModelNames();


	};

}
