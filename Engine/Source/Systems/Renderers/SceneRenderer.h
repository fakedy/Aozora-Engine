#pragma once
#include <unordered_map>
#include "Opengl/OpenglFrameBuffer.h"
#include "Viewport.h"
#include <Systems/Renderers/DeferredPipeline.h>
#include "IrenderAPI.h"
#include <Systems/Events/Event.h>
#include <Systems/Events/Events.h>

namespace Aozora {


	class SceneRenderer : public IEventListener{
	public:

		SceneRenderer();



		std::unique_ptr<IrenderAPI> m_RenderAPI;
		std::unordered_map<uint32_t, Viewport> m_viewports;
		uint32_t nextViewportID{ 0 };
		void render();

		uint32_t createViewport(ViewportType type);

		Viewport& getViewport(uint32_t viewportID);

		void updatePrimaryScene(Scene& scene);

		void onEvent(Event& e) override;
	};
}
