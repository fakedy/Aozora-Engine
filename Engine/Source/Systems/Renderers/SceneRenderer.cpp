#include "SceneRenderer.h"
#include "OpenGL.h"



namespace Aozora {
	SceneRenderer::SceneRenderer()
	{

		m_RenderAPI = std::make_unique<OpenGL>();
	}

	void SceneRenderer::updatePrimaryScene(Scene* scene)
	{

		for (auto& [ID, viewport] : m_viewports) {

			if (viewport.type == ViewportType::PrimaryGame || viewport.type == ViewportType::PrimaryEditor) {
				viewport.scene = scene;
			}

		}
	}


	void SceneRenderer::render() {

		// loop through unordered_map of viewports
		for (auto& [ID, viewport] : m_viewports) {

			// if viewport doesnt have any scene
			if (!viewport.scene) {
				continue;
			}

			viewport.renderPipeline->execute(*m_RenderAPI, *viewport.scene, viewport.camera, viewport.width, viewport.height);
		}
	}


	uint32_t SceneRenderer::createViewport(Scene* scene, ViewportType type, entt::entity editorCameraEntity)
	{
		uint32_t viewportID = nextViewportID;


		m_viewports.emplace(std::piecewise_construct, std::forward_as_tuple(viewportID), std::forward_as_tuple(1920, 1080, std::make_unique<DeferredPipeline>(1920, 1080)));
		Viewport& tempViewport = m_viewports.at(viewportID);
		tempViewport.type = type;
		tempViewport.scene = scene;
		tempViewport.camera = editorCameraEntity;
		nextViewportID++;
		return viewportID;
	}

	Viewport& SceneRenderer::getViewport(uint32_t viewportID)
	{
		return m_viewports.find(viewportID)->second;
	}
}
