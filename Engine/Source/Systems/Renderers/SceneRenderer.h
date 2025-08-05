#pragma once
#include <unordered_map>
#include "Opengl/OpenglFrameBuffer.h"
#include "Viewport.h"
#include <Systems/Renderers/DeferredPipeline.h>
#include "IrenderAPI.h"
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>

namespace Aozora::Graphics {


	class SceneRenderer : public IEventListener{
	public:

		SceneRenderer(IrenderAPI* api);



		IrenderAPI* m_RenderAPI;
		std::unordered_map<uint32_t, Viewport> m_viewports;
		uint32_t nextViewportID{ 0 };
		void render();

		uint32_t createViewport(ViewportType type);

		// the function name and the bool is quite dumb
		void setViewportActive(uint32_t ID, bool condition);
		void resizeViewport(uint32_t ID, uint16_t width, uint32_t height);

		Viewport& getViewport(uint32_t viewportID);
		uint32_t getViewportTextureID(uint32_t ID);

		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };
		OpenglShader m_gBufferShader{ OpenglShader("Resources/Shaders/v_gBuffer.glsl", "Resources/Shaders/f_gBuffer.glsl") };
		OpenglShader m_postfxShader{ OpenglShader("Resources/Shaders/v_postfxShader.glsl", "Resources/Shaders/f_postfxShader.glsl") };
		OpenglShader m_skyboxShader{ OpenglShader("Resources/Shaders/v_cubemap.glsl", "Resources/Shaders/f_cubemap.glsl") };

		void updatePrimaryScene(Scene& scene);

		void onEvent(Event& e) override;

		struct PSO {
		public:
			// pipeline state object
			/*
			* Role of the PSO is to set the necessary states before drawing.
			*/

			std::function<void()> commands;

			void bind() {
				// set the necessary states


				commands();

			}
		};

		struct RenderCommand {
			uint32_t countL;
			uint32_t instanceCount;
			uint32_t firstIndex;
			uint32_t baseVertex;
			uint32_t baseInstance;
		};
	};
}
