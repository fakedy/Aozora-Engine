#include "DeferredPipeline.h"
#include "Application.h"

namespace Aozora {

	DeferredPipeline::DeferredPipeline(uint32_t width, uint32_t height) {


		FrameBuffer::FrameBufferSpecification gBufferSpecs;
		FrameBuffer::FrameBufferAttachment positionAttachment;
		FrameBuffer::FrameBufferAttachment normalAttachment;
		FrameBuffer::FrameBufferAttachment albedoAttachment;	
		FrameBuffer::FrameBufferAttachment emissiveAttachment;
		FrameBuffer::FrameBufferAttachment propertiesAttachment;
		FrameBuffer::FrameBufferAttachment gBufferDepthAttachment;

		gBufferSpecs.width = 1920;
		gBufferSpecs.height = 1080;

		positionAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		positionAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		positionAttachment.dataType = FrameBuffer::DataType::FLOAT;
		positionAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		normalAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		normalAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		normalAttachment.dataType = FrameBuffer::DataType::FLOAT;
		normalAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		albedoAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		albedoAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		albedoAttachment.dataType = FrameBuffer::DataType::FLOAT;
		albedoAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		emissiveAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		emissiveAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		emissiveAttachment.dataType = FrameBuffer::DataType::FLOAT;
		emissiveAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		propertiesAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		propertiesAttachment.textureFilter = FrameBuffer::TextureFilter::Nearest;
		propertiesAttachment.dataType = FrameBuffer::DataType::FLOAT;
		propertiesAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

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

		FrameBuffer::FrameBufferSpecification renderBufferSpecs;
		FrameBuffer::FrameBufferAttachment colorAttachment;
		FrameBuffer::FrameBufferAttachment depthAttachment;

		renderBufferSpecs.width = 1920;
		renderBufferSpecs.height = 1080;

		colorAttachment.textureFormat = FrameBuffer::TextureFormat::RGBA16F;
		colorAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		colorAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		colorAttachment.dataType = FrameBuffer::DataType::FLOAT;
		colorAttachment.dataFormat = FrameBuffer::DataFormat::RGBA;

		depthAttachment.textureFormat = FrameBuffer::TextureFormat::DEPTH24STENCIL8;
		depthAttachment.textureFilter = FrameBuffer::TextureFilter::Linear;
		depthAttachment.textureWrap = FrameBuffer::TextureWrap::ClampToEdge;
		depthAttachment.dataType = FrameBuffer::DataType::UNSIGNED_INT_24_8;
		depthAttachment.dataFormat = FrameBuffer::DataFormat::DEPTH_STENCIL;

		renderBufferSpecs.attachments.push_back(colorAttachment);
		renderBufferSpecs.attachments.push_back(depthAttachment);

		gBuffer = std::make_unique<OpenglFrameBuffer>(gBufferSpecs);
		renderBuffer = std::make_unique<OpenglFrameBuffer>(renderBufferSpecs);

		gBuffer->buffer();
		renderBuffer->buffer();

		// i dont care, we dont need normals but whatever
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

	}




	void DeferredPipeline::resize(uint32_t width, uint32_t height)
	{

		gBuffer->updateTexture(width, height);
		renderBuffer->updateTexture(width, height);

	}

	void DeferredPipeline::execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height)
	{

		auto view = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
		auto cameraView = scene.getRegistry().view<CameraComponent>();

		// check if viewport have a camera
		if (camera != entt::null) {
			// gBuffer pass
			gBuffer->bind();
			ResourceManager& resourceManager = Application::getApplication().getResourceManager();

			glClearColor(0.0, 0.0, 0.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(m_gBufferShader.ID);
			glViewport(0, 0, width, height);

			auto& current_camera = cameraView.get<CameraComponent>(camera);
			current_camera.m_viewPortWidth = width;
			current_camera.m_viewPortHeight = height;

			glDisable(GL_BLEND);

			// for every entity to be rendered
			for (const auto entity : view) {
				auto& meshComponent = view.get<MeshComponent>(entity);
				auto& transformComponent = view.get<TransformComponent>(entity);

				// get rid of "glgetuniformlocation"
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "model"), 1, GL_FALSE, &transformComponent.model[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "view"), 1, GL_FALSE, &current_camera.getView()[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_gBufferShader.ID, "proj"), 1, GL_FALSE, &current_camera.getProjection()[0][0]);


				// draw call
				resourceManager.m_loadedMeshes[meshComponent.meshID].draw(m_gBufferShader);

			}
			glEnable(GL_BLEND);

			gBuffer->unbind();
			renderBuffer->bind();
			// light pass
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glUseProgram(m_defaultShader.ID);


			auto& camera_transform = view.get<TransformComponent>(camera);
			glUniform3fv(glGetUniformLocation(m_gBufferShader.ID, "cameraPos"), 1, &camera_transform.pos[0]);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gPosition"), 0);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gNormal"), 1);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gAlbedo"), 2);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gEmissive"), 3);
			glUniform1i(glGetUniformLocation(m_defaultShader.ID, "gProperties"), 4);

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

			// render lightss


			screenQuad.drawGeometry();


			renderBuffer->unbind();

		}




	}
	uint32_t DeferredPipeline::getFinalImage()
	{
		return renderBuffer->m_colorAttachments[0];
	}
}
