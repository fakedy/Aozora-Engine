#include "SceneRenderer.h"
#include "OpenGL.h"
#include <Systems/ECS/Components/Components.h>
#include <Systems/Logging/Logger.h>

namespace Aozora::Graphics {
	SceneRenderer::SceneRenderer(IrenderAPI* api, SceneManager& sceneManager) : m_sceneManager(sceneManager)
	{

		m_RenderAPI = api;

		EventDispatcher::subscribe(EventType::NewMesh, [this](Event& e) {
			this->onEvent(e);
			});

	}

	void SceneRenderer::updatePrimaryScene(uint64_t sceneID)
	{
		Scene* scene = m_sceneManager.getCurrentActiveScene();
		auto view = scene->getRegistry().view<CameraComponent, EditorEntityTag>();
		
		for (auto& [ID, viewport] : m_viewports) {

			if (viewport.type == ViewportType::PrimaryEditor) {
				viewport.setScene(scene->hash);
				viewport.camera = view.front(); // temp
			}
		}
		Log::info("Updated primary scene");
	}

	void SceneRenderer::onEvent(Event& e)
	{
		switch (e.getEventType()) {
		case(EventType::NewMesh):
		{
			auto* scene = static_cast<EntityCreatedWithMeshEvent&>(e).getScene();
			// find viewport with this scene
			// kinda whack
			for (auto& [ID, viewport] : m_viewports) {
				if (viewport.sceneID == scene->hash) {
					viewport.renderPipeline->genMegaBuffer(*scene);
				}
			}
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
			Scene* scene = m_sceneManager.getCurrentActiveScene();
			if (scene != nullptr && viewport.isActive) {
				viewport.renderPipeline->execute(*m_RenderAPI, *scene, viewport.camera, viewport.width, viewport.height);
			}
		}
	}

	// note
	// make this class listen on all scenes, if a scene add a mesh we update the megabuffer

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
