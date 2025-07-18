#pragma once 
#include <memory>
#include "FrameBuffer.h"
#include "Systems/Scene/Scene.h"
#include <entt/entt.hpp>
#include "Systems/Renderers/IrenderPipeline.h"
#include "Systems/Renderers/DeferredPipeline.h"
#include <Systems/Mesh.h>

namespace Aozora {

		enum class ViewportType { Generic, PrimaryGame, PrimaryEditor };

	class Viewport {
	public:

		Viewport(uint32_t width, uint32_t height, std::unique_ptr<IrenderPipeline> pipeline);

		
		ViewportType type{ ViewportType::Generic };
		uint32_t width{1920};
		uint32_t height{1080};

		std::unique_ptr<IrenderPipeline> renderPipeline;
		

		Scene* scene;
		entt::entity camera;

		void resize(uint32_t width, uint32_t height);

	};
}
