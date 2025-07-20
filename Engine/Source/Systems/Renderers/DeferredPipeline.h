#pragma once 

#include "IrenderPipeline.h"
#include "FrameBuffer.h"
#include "Opengl/OpenglFrameBuffer.h"
#include <memory>


namespace Aozora {

	class DeferredPipeline : public IrenderPipeline {
	public:

		DeferredPipeline(uint32_t width, uint32_t height);

		 void resize(uint32_t width, uint32_t height) override;
		void execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height) override;

		uint32_t getFinalImage() override;
		std::unique_ptr<FrameBuffer> renderBuffer;


	private:
		std::unique_ptr<FrameBuffer> gBuffer;
		Mesh screenQuad;

		// maybe call a function in the api to create a shader from given strings.
		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };

		OpenglShader m_gBufferShader{ OpenglShader("Resources/Shaders/v_gBuffer.glsl", "Resources/Shaders/f_gBuffer.glsl") };
		OpenglShader m_poopShader{ OpenglShader("Resources/Shaders/v_poopshader.glsl", "Resources/Shaders/f_poopshader.glsl") };


		void renderLights(Scene& scene);
	};


}