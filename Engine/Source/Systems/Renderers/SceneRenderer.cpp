#include "SceneRenderer.h"
#include "OpenGL.h"
#include <Systems/ECS/Components/Components.h>
#include <functional>


namespace Aozora::Graphics {
	SceneRenderer::SceneRenderer(IrenderAPI* api)
	{

		m_RenderAPI = api;

		EventDispatcher::subscribe(EventType::ChangeScene, [this](Event& e) {
			this->onEvent(e);
		});

		PSO gbufferPSO;
		gbufferPSO.commands = []() {
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDisable(GL_BLEND);
		};
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
		switch (e.getEventType()) {
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

	// prototyping
	/*
	* If i get this straight, we can create a command for all the transparent and all the opaque objects?
	* and use glMultiDrawIndirect 
	*/




	void SceneRenderer::render() {

		// loop through unordered_map of viewports
		for (auto& [ID, viewport] : m_viewports) {
			
			// make sure viewport is active before doing this.
			// if viewport have a scene
			if (viewport.scene != nullptr && viewport.isActive) {

				// submit render command

				std::vector<RenderCommand> commands;


				
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

	void SceneRenderer::setViewportActive(uint32_t ID, bool condition)
	{
		Viewport& viewport = getViewport(ID);

		viewport.isActive = condition;
	}

	void SceneRenderer::resizeViewport(uint32_t ID, uint16_t width, uint32_t height)
	{
		Viewport& viewport = getViewport(ID);

		viewport.resize(width, height);
	}

	Viewport& SceneRenderer::getViewport(uint32_t viewportID)
	{
		return m_viewports.find(viewportID)->second;
	}
	uint32_t SceneRenderer::getViewportTextureID(uint32_t ID)
	{
		Viewport& viewport = getViewport(ID);

		return viewport.renderPipeline->getFinalImage();
	}
}
