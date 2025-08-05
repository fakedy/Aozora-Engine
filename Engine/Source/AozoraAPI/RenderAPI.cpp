#include <AozoraAPI/Aozora.h>
#include <Application.h>
#include <Systems/Renderers/FrameBuffer.h>
#include <Systems/Renderers/OpenGL.h>
#include <Systems/Renderers/Viewport.h>
#include <Systems/Renderers/SceneRenderer.h>


namespace Aozora {

	// create viewport with set camera and returns the ID
	uint32_t RenderAPI::createViewport(ViewportType type, entt::entity editorCameraEntity) {

		Graphics::SceneRenderer& renderer = Application::getApplication().getRenderer();

		uint32_t viewportID = renderer.createViewport(type);
		return viewportID;
	}

	// create viewport without set camera and returns the ID
	uint32_t RenderAPI::createViewport(ViewportType type) {

		Graphics::SceneRenderer& renderer = Application::getApplication().getRenderer();

		uint32_t viewportID = renderer.createViewport(type);
		return viewportID;
	}



}