#include <AozoraAPI/Aozora.h>
#include <Application.h>
#include <Systems/Renderers/FrameBuffer.h>
#include <Systems/Renderers/OpenGL.h>
#include <Systems/Renderers/Viewport.h>
#include <Systems/Renderers/SceneRenderer.h>


namespace Aozora {

	// create viewport with set camera and returns the ID
	uint32_t RenderAPI::createViewport(Scene* scene, ViewportType type, entt::entity editorCameraEntity) {

		SceneRenderer& renderer = Application::getApplication().getRenderer();

		uint32_t viewportID = renderer.createViewport(scene, type, editorCameraEntity);
		return viewportID;
	}

	// create viewport without set camera and returns the ID
	uint32_t RenderAPI::createViewport(Scene* scene, ViewportType type) {

		SceneRenderer& renderer = Application::getApplication().getRenderer();

		uint32_t viewportID = renderer.createViewport(scene, type, entt::null);
		return viewportID;
	}

	void RenderAPI::resizeViewport(uint32_t ID, uint16_t width, uint32_t height)
	{

		SceneRenderer& renderer = Application::getApplication().getRenderer();

		Viewport& viewport = renderer.getViewport(ID);
		
		viewport.resize(width, height);


	}

	uint32_t RenderAPI::getViewportTextureID(uint32_t ID)
	{
		SceneRenderer& renderer = Application::getApplication().getRenderer();

		Viewport& viewport = renderer.getViewport(ID);

		return viewport.renderPipeline->getFinalImage();
	}


}