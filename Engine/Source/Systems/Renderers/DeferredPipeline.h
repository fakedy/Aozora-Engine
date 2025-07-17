#pragma once 

#include "IrenderPipeline.h"
#include "FrameBuffer.h"
#include "Opengl/OpenglFrameBuffer.h"
#include <memory>


namespace Aozora {

	class DeferredPipeline : public IrenderPipeline {
	public:

		DeferredPipeline(uint32_t width, uint32_t height);


		void createFramebuffer() override;
		void execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height) override;

		uint32_t getFinalImage() override;
		std::unique_ptr<FrameBuffer> renderBuffer;


	private:
		std::unique_ptr<FrameBuffer> gBuffer;


		// maybe call a function in the api to create a shader from given strings.
		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };

	};


}