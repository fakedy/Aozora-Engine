#pragma once 
#include <memory>
#include "FrameBuffer.h"
#include "Systems/Scene/Scene.h"
#include <entt/entt.hpp>


namespace Aozora {

		enum class ViewportType { Generic, PrimaryGame, PrimaryEditor };

	class Viewport {
	public:

		Viewport(uint32_t width, uint32_t height);

		
		ViewportType type{ ViewportType::Generic };
		uint32_t width{1920};
		uint32_t height{1080};

		std::unique_ptr<FrameBuffer> framebuffer;

		Scene* scene;
		entt::entity camera;

	};
}
