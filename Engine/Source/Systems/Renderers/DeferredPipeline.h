#pragma once 

#include "IrenderPipeline.h"
#include "FrameBuffer.h"
#include "Opengl/OpenglFrameBuffer.h"
#include <memory>
#include <Systems/Mesh.h>

namespace Aozora {

	class DeferredPipeline : public IrenderPipeline {
	public:

		DeferredPipeline(uint32_t width, uint32_t height);

		 void resize(uint32_t width, uint32_t height) override;
		void execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height) override;

		uint32_t getFinalImage() override;
		std::unique_ptr<FrameBuffer> renderBuffer;

		void genMegaBuffer(Scene& scene);
		void updateMegaBuffer(Scene& scene);

	private:
		std::unique_ptr<FrameBuffer> gBuffer;
		std::unique_ptr<FrameBuffer> postfxBuffer;
		std::unique_ptr<FrameBuffer> irradianceBuffer;
		Mesh screenQuad;
		Mesh skybox;

		uint32_t m_indirectBuffer;
		uint32_t m_VAO;
		uint32_t m_VBO;
		uint32_t m_EBO;

		uint32_t objectSSBO;

		struct ObjectData {
			glm::mat4 model;
			uint64_t diffuseTextureHandle;
			uint64_t emissiveTextureHandle;
			uint64_t aoTextureHandle;
			uint64_t metallicTextureHandle;
			uint64_t roughnessTextureHandle;
			uint64_t normalTextureHandle;
			glm::vec4 albedo;
			glm::vec4 emissive;
			glm::vec3 normal;
			float pad1;
			float metallic;
			float roughness;
			float ao;
			float pad2;
		};
		

		

		// maybe call a function in the api to create a shader from given strings.
		OpenglShader m_defaultShader{ OpenglShader("Resources/Shaders/v_default.glsl", "Resources/Shaders/f_default.glsl") };
		OpenglShader m_gridShader{ OpenglShader("Resources/Shaders/v_grid.glsl", "Resources/Shaders/f_grid.glsl") };
		OpenglShader m_gBufferShader{ OpenglShader("Resources/Shaders/v_gBuffer.glsl", "Resources/Shaders/f_gBuffer.glsl") };
		OpenglShader m_postfxShader{ OpenglShader("Resources/Shaders/v_postfxShader.glsl", "Resources/Shaders/f_postfxShader.glsl") };
		OpenglShader m_skyboxShader{ OpenglShader("Resources/Shaders/v_cubemap.glsl", "Resources/Shaders/f_cubemap.glsl") };


		void setupGbuffer();
		void setupPostfxBuffer();
		void setupRenderBuffer();

		void renderLights(Scene& scene);
		void postfxPass();

		uint32_t m_outputAttachment{ 0 };

	};


}