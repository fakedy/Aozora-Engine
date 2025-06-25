#include <AozoraAPI/Aozora.h>
#include <Application.h>
#include <Systems/Renderers/FrameBuffer.h>
#include <Systems/Renderers/OpenGL.h>
#include <Systems/Renderers/Viewport.h>


namespace Aozora {


	uint32_t RenderAPI::createViewport(Scene* scene, ViewportType type, entt::entity editorCameraEntity) {

		Renderer& renderer = Application::getApplication().getRenderer();
		// for now we "know" that we use opengl
		OpenGL& openglRenderer = static_cast<OpenGL&>(renderer);


		uint32_t viewportID = openglRenderer.createViewport(scene, type, editorCameraEntity);
		return viewportID;
	}

	uint32_t RenderAPI::createViewport(Scene* scene, ViewportType type) {

		Renderer& renderer = Application::getApplication().getRenderer();
		// for now we "know" that we use opengl
		OpenGL& openglRenderer = static_cast<OpenGL&>(renderer);

		uint32_t viewportID = openglRenderer.createViewport(scene, type, entt::null);
		return viewportID;
	}

	void RenderAPI::resizeViewport(uint32_t ID, uint16_t width, uint32_t height)
	{

		Renderer& renderer = Application::getApplication().getRenderer();
		// for now we "know" that we use opengl
		OpenGL& openglRenderer = static_cast<OpenGL&>(renderer);

		Viewport& viewport = openglRenderer.getViewport(ID);
		
		if (viewport.width != width || viewport.height != height) {

			viewport.width = width;
			viewport.height = height;
			viewport.framebuffer->updateTexture(width, height);
		}


	}

	uint32_t RenderAPI::getViewportTextureID(uint32_t ID)
	{
		Renderer& renderer = Application::getApplication().getRenderer();
		// for now we "know" that we use opengl
		OpenGL& openglRenderer = static_cast<OpenGL&>(renderer);

		Viewport& viewport = openglRenderer.getViewport(ID);

		return viewport.framebuffer->m_colorTextureID;
	}


}