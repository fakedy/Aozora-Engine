#include "SceneRenderer.h"
#include "OpenGL.h"
#include <Systems/ECS/Components/Components.h>


namespace Aozora {
	SceneRenderer::SceneRenderer()
	{

		m_RenderAPI = std::make_unique<OpenGL>();

		EventDispatcher::subscribe(EventType::ChangeScene, [this](Event& e) {
			this->onEvent(e);
		});
	}

	void SceneRenderer::updatePrimaryScene(Scene& scene)
	{

		auto view = scene.getRegistry().view<CameraComponent, EditorEntityTag>();


		for (auto& [ID, viewport] : m_viewports) {

			if (viewport.type == ViewportType::PrimaryGame || viewport.type == ViewportType::PrimaryEditor) {
				viewport.scene = &scene;
				viewport.camera = view.front(); // temp
			}

			if (viewport.type == ViewportType::PrimaryEditor) {

				viewport.camera = view.front(); // should ideally be the only editor camera
			}
		}
	}


	// doesnt get called because who knows
	void SceneRenderer::onEvent(Event& e)
	{
		switch (e.getEvent()) {
		case(EventType::ChangeScene):
		{
			auto& scene = static_cast<ChangeSceneEvent&>(e).getScene();
			updatePrimaryScene(scene);
			std::cout << "scene changed\n";
			break;
		}

		default:
			break;
		}

	}




	void SceneRenderer::render() {

		// loop through unordered_map of viewports
		for (auto& [ID, viewport] : m_viewports) {
			
			// make sure viewport is active before doing this.
			// if viewport have a scene
			if (viewport.scene != nullptr && viewport.isActive) {
				viewport.renderPipeline->execute(*m_RenderAPI, *viewport.scene, viewport.camera, viewport.width, viewport.height);
			}
		}
	}


	uint32_t SceneRenderer::createViewport(ViewportType type)
	{
		uint32_t viewportID = nextViewportID;


		m_viewports.emplace(std::piecewise_construct, std::forward_as_tuple(viewportID), std::forward_as_tuple(1920, 1080, std::make_unique<DeferredPipeline>(1920, 1080)));
		Viewport& tempViewport = m_viewports.at(viewportID);
		tempViewport.type = type;
		nextViewportID++;
		return viewportID;
	}

	Viewport& SceneRenderer::getViewport(uint32_t viewportID)
	{
		return m_viewports.find(viewportID)->second;
	}
}
