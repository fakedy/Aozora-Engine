#include "DeferredPipeline.h"
#include "Application.h"

namespace Aozora {

	DeferredPipeline::DeferredPipeline(uint32_t width, uint32_t height) {

		// setting up framebuffers for deferred rendering
		setupGbuffer();
		setupRenderBuffer();
		setupPostfxBuffer();

		// We dont need normals but whatever
		screenQuad.meshData.vertices = {
			{ {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f} }, // 0: Bottom-left
			{ { 1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f} }, // 1: Bottom-right
			{ {-1.0f,  1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {0.0f, 1.0f} }, // 2: Top-left
			{ { 1.0f,  1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {-1.0f, -1.0f, 0.0f}, {1.0f, 1.0f} }  // 3: Top-right
		};

		screenQuad.meshData.indices = {
			0, 1, 3, // First triangle
			3, 2, 0  // Second triangle
		};

		screenQuad.bufferData();

		// illegal type stuff
		glGenVertexArrays(1, &skybox.VAO);


	}




	void DeferredPipeline::resize(uint32_t width, uint32_t height)
	{
		gBuffer->updateTexture(width, height);
		renderBuffer->updateTexture(width, height);
		postfxBuffer->updateTexture(width, height);

	}

	void DeferredPipeline::execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height)
	{

		auto MeshTransformEntities = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
		auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
		auto cameraView = scene.getRegistry().view<CameraComponent>();

		// check if viewport have a camera
		if (camera != entt::null) {
			// gBuffer pass
			gBuffer->bind();
			ResourceManager& resourceManager = Application::getApplication().getResourceManager();


			renderAPI.clear(0.0f, 0.0f, 0.0f, 1.0f);
			glUseProgram(m_gBufferShader.ID);
			glViewport(0, 0, width, height);

			// get the camera from the cameraID
			auto& current_camera = cameraView.get<CameraComponent>(camera);
			current_camera.m_viewPortWidth = width;
			current_camera.m_viewPortHeight = height;

			glDisable(GL_BLEND);

			// for every entity to be rendered
			for (const auto entity : MeshTransformEntities) {
				auto& meshComponent = MeshTransformEntities.get<MeshComponent>(entity);
				auto& transformComponent = MeshTransformEntities.get<TransformComponent>(entity);

				// get rid of "glgetuniformlocation"
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "model"), 1, GL_FALSE, &transformComponent.model[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "view"), 1, GL_FALSE, &current_camera.getView()[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "proj"), 1, GL_FALSE, &current_camera.getProjection()[0][0]);


				// draw call
				resourceManager.m_loadedMeshes[meshComponent.meshID].draw(m_gBufferShader);

			}
			//glEnable(GL_BLEND);

			gBuffer->unbind();

			// copy the depth buffer to the lighting buffer
			glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer->framebufferID);
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, renderBuffer->framebufferID);

			glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			renderBuffer->bind();
			// light pass

			glDisable(GL_BLEND);
			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			glUseProgram(m_defaultShader.ID);

			auto& camera_transform = MeshTransformEntities.get<TransformComponent>(camera);
			glUniform3fv(glGetUniformLocation(m_gBufferShader.ID, "cameraPos"), 1, &camera_transform.pos[0]);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gPosition"), 0);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gNormal"), 1);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gAlbedo"), 2);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gEmissive"), 3);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gProperties"), 4);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gDepth"), 5);

			auto skyboxes = scene.getRegistry().view<const SkyboxComponent>();
			auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skyboxes[0]); // hack

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[0]);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[1]);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[2]);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[3]);
			glActiveTexture(GL_TEXTURE4);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_colorAttachments[4]);
			glActiveTexture(GL_TEXTURE5);
			glBindTexture(GL_TEXTURE_2D, gBuffer->m_depthTextureID);
			glActiveTexture(GL_TEXTURE6);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxComponent.data.irradianceTextureID);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "irradianceMap"), 6);

			glDepthMask(GL_FALSE);
			renderLights(scene);
			glDepthMask(GL_TRUE);

			// render skybox
			glDepthFunc(GL_LEQUAL);
			glDisable(GL_CULL_FACE);
			glUseProgram(m_skyboxShader.ID);
			glBindVertexArray(skybox.VAO);
			for (const auto skybox : skyboxes) {
				glUniform1i(glGetUniformLocation(m_skyboxShader.ID, "skybox"), 0);
				auto& skyboxComponent = skyboxes.get<SkyboxComponent>(skybox);
				glm::mat4 viewWithoutRotation = glm::mat4(glm::mat3(current_camera.getView()));
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxComponent.data.skyboxTextureID);
				glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader.ID, "view"), 1, GL_FALSE, &viewWithoutRotation[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_skyboxShader.ID, "proj"), 1, GL_FALSE, &current_camera.getProjection()[0][0]);

				glDrawArrays(GL_TRIANGLES, 0, 36);
				glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

			}
			glBindVertexArray(0);
			glDepthFunc(GL_LESS);
			glEnable(GL_CULL_FACE);
			glEnable(GL_BLEND);
			renderBuffer->unbind();

			postfxBuffer->bind();
			postfxPass();
			postfxBuffer->unbind();

		}

	}
	uint32_t DeferredPipeline::getFinalImage()
	{
		return postfxBuffer->m_colorAttachments[0];
	}
	void DeferredPipeline::setupGbuffer()
	{
		FrameBuffer::FrameBufferSpecification gBufferSpecs;
		FrameBuffer::FrameBufferAttachment positionAttachment;
		FrameBuffer::FrameBufferAttachment normalAttachment;
		FrameBuffer::FrameBufferAttachment albedoAttachment;
		FrameBuffer::FrameBufferAttachment emissiveAttachment;
		FrameBuffer::FrameBufferAttachment propertiesAttachment;
		FrameBuffer::FrameBufferAttachment gBufferDepthAttachment;

		gBufferSpecs.width = 1920;
		gBufferSpecs.height = 1080;

		positionAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		positionAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		positionAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		positionAttachment.dataType = FrameBuffer::DataType::FLOAT;
		positionAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		normalAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		normalAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		normalAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		normalAttachment.dataType = FrameBuffer::DataType::FLOAT;
		normalAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		albedoAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		albedoAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		albedoAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		albedoAttachment.dataType = FrameBuffer::DataType::FLOAT;
		albedoAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		emissiveAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		emissiveAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		emissiveAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		emissiveAttachment.dataType = FrameBuffer::DataType::FLOAT;
		emissiveAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		propertiesAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		propertiesAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA8;
		propertiesAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		propertiesAttachment.dataType = FrameBuffer::DataType::FLOAT;
		propertiesAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		gBufferDepthAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		gBufferDepthAttachment.textureFormat = FrameBuffer::TextureFormat::DEPTH24STENCIL8;
		gBufferDepthAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		gBufferDepthAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		gBufferDepthAttachment.dataType = FrameBuffer::DataType::UNSIGNED_INT_24_8;
		gBufferDepthAttachment.dataFormat = FrameBuffer::DataFormat::DEPTH_STENCIL;


		gBufferSpecs.attachments.push_back(positionAttachment);
		gBufferSpecs.attachments.push_back(normalAttachment);
		gBufferSpecs.attachments.push_back(albedoAttachment);
		gBufferSpecs.attachments.push_back(emissiveAttachment);
		gBufferSpecs.attachments.push_back(propertiesAttachment);
		gBufferSpecs.attachments.push_back(gBufferDepthAttachment);
		gBuffer = std::make_unique<OpenglFrameBuffer>(gBufferSpecs);

		gBuffer->buffer();
	}
	void DeferredPipeline::setupPostfxBuffer()
	{
		FrameBuffer::FrameBufferSpecification postfxBufferSpecs;
		FrameBuffer::FrameBufferAttachment postfxColorAttachment;
		postfxBufferSpecs.width = 1920;
		postfxBufferSpecs.height = 1080;

		postfxColorAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		postfxColorAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		postfxColorAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		postfxColorAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		postfxColorAttachment.dataType = FrameBuffer::DataType::FLOAT;
		postfxColorAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		postfxBufferSpecs.attachments.push_back(postfxColorAttachment);

		postfxBuffer = std::make_unique<OpenglFrameBuffer>(postfxBufferSpecs);
		postfxBuffer->buffer();
	}
	void DeferredPipeline::setupRenderBuffer()
	{
		FrameBuffer::FrameBufferSpecification renderBufferSpecs;
		FrameBuffer::FrameBufferAttachment colorAttachment;
		FrameBuffer::FrameBufferAttachment depthAttachment;

		renderBufferSpecs.width = 1920;
		renderBufferSpecs.height = 1080;

		colorAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		colorAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		colorAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		colorAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		colorAttachment.dataType = FrameBuffer::DataType::FLOAT;
		colorAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		depthAttachment.textureTarget = FrameBuffer::TextureTarget::TEXTURE_2D;
		depthAttachment.textureFormat = FrameBuffer::TextureFormat::DEPTH24STENCIL8;
		depthAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		depthAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		depthAttachment.dataType = FrameBuffer::DataType::UNSIGNED_INT_24_8;
		depthAttachment.dataFormat = FrameBuffer::DataFormat::DEPTH_STENCIL;

		renderBufferSpecs.attachments.push_back(colorAttachment);
		renderBufferSpecs.attachments.push_back(depthAttachment);

		renderBuffer = std::make_unique<OpenglFrameBuffer>(renderBufferSpecs);
		renderBuffer->buffer();
	}
	void DeferredPipeline::renderLights(Scene& scene)
	{
		// render lightss
		auto lightView = scene.getRegistry().view<const LightComponent, TransformComponent>();

		int index = 0;
		for (auto entity : lightView) {
			auto& lightComponent = lightView.get<LightComponent>(entity);
			auto& transformComponent = lightView.get<TransformComponent>(entity);
			std::string lightPosVar = "lights[" + std::to_string(index) + "].position";
			std::string lightColorVar = "lights[" + std::to_string(index) + "].color";
			std::string lightLinearVar = "lights[" + std::to_string(index) + "].linear";
			std::string lightQuadraticVar = "lights[" + std::to_string(index) + "].quadratic";
			std::string lightRadiusVar = "lights[" + std::to_string(index) + "].radius";
			std::string lightPowerVar = "lights[" + std::to_string(index) + "].power";
			glUniform3fv(glGetUniformLocation(m_defaultShader.ID, lightPosVar.c_str()), 1, &transformComponent.pos[0]);
			glUniform3fv(glGetUniformLocation(m_defaultShader.ID, lightColorVar.c_str()), 1, &lightComponent.color[0]);
			glUniform1f(glGetUniformLocation(m_defaultShader.ID, lightLinearVar.c_str()), lightComponent.linear);
			glUniform1f(glGetUniformLocation(m_defaultShader.ID, lightQuadraticVar.c_str()), lightComponent.quadratic);
			glUniform1f(glGetUniformLocation(m_defaultShader.ID, lightRadiusVar.c_str()), lightComponent.radius);
			glUniform1f(glGetUniformLocation(m_defaultShader.ID, lightPowerVar.c_str()), lightComponent.power);
			index++;
		}
		glUniform1i(glGetUniformLocation(m_defaultShader.ID, "activeLights"), index);

		screenQuad.drawGeometry();

	}
	void DeferredPipeline::postfxPass()
	{
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(m_postfxShader.ID);

		glActiveTexture(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(m_postfxShader.ID, "colorTexture"), 0);
		glBindTexture(GL_TEXTURE_2D, renderBuffer->m_colorAttachments[0]);
		screenQuad.drawGeometry();
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}
