#include "DeferredPipeline.h"
#include "Application.h"

namespace Aozora {

	DeferredPipeline::DeferredPipeline(uint32_t width, uint32_t height) {

		gBuffer = std::make_unique<OpenglFrameBuffer>();
		renderBuffer = std::make_unique<OpenglFrameBuffer>();


		renderBuffer->create();
		renderBuffer->createTextures(1920, 1080);
		renderBuffer->bufferTexture();

	}



	void DeferredPipeline::createFramebuffer()
	{




	}
	void DeferredPipeline::execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height)
	{
		
		//gBuffer->bind();
		renderBuffer->bind();

		// clear
		ResourceManager& resourceManager = Application::getApplication().getResourceManager();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.0f, 0.51f, 0.6f, 1.0f);

		glUseProgram(m_defaultShader.ID);
		glViewport(0, 0, width, height);
		auto view = scene.getRegistry().view<const MeshComponent, TransformComponent>(); // register of all mesh components
		auto cameraView = scene.getRegistry().view<CameraComponent>();

		// check if viewport have a camera
		if (camera != entt::null) {
			auto& current_camera = cameraView.get<CameraComponent>(camera);
			current_camera.m_viewPortWidth = width;
			current_camera.m_viewPortHeight = height;

			// for every entity to be rendered
			for (const auto entity : view) {
				auto& meshComponent = view.get<MeshComponent>(entity);
				auto& transformComponent = view.get<TransformComponent>(entity);

				// get rid of "glgetuniformlocation"?
				glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "model"), 1, GL_FALSE, &transformComponent.model[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "view"), 1, GL_FALSE, &current_camera.getView()[0][0]);
				glUniformMatrix4fv(glGetUniformLocation(m_defaultShader.ID, "proj"), 1, GL_FALSE, &current_camera.getProjection()[0][0]);

				glUniform3fv(glGetUniformLocation(m_defaultShader.ID, "cameraPos"), 1, &transformComponent.pos[0]);

				// draw call
				resourceManager.m_loadedMeshes[meshComponent.meshID].draw(m_defaultShader);

			}
		}

		renderBuffer->unbind();
		//gBuffer->unbind();

		// clear


	}
	uint32_t DeferredPipeline::getFinalImage()
	{
		return renderBuffer->m_colorTextureID;
	}
}
