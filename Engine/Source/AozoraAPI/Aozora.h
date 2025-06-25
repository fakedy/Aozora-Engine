#pragma once
// Includes used by applications

#include "Application.h"
#include "Systems/Layers/ImguiLayer.h"
#include "Systems/Layers/Layer.h"
#include "Systems/Renderers/Viewport.h"
#include "entt/entt.hpp"

namespace Aozora {


	// api


	class SceneAPI {
	public:

		static void addEntity();
		static void deleteEntity();
		static std::vector<entt::entity> getSceneHierarchyEntities();
		static std::string getEntityName(entt::entity entity);


	};

	class RenderAPI {
	public:

		static uint32_t createViewport(Scene* scene, ViewportType type, entt::entity editorCameraEntity);
		static uint32_t createViewport(Scene* scene, ViewportType type);
		static void resizeViewport(uint32_t ID, uint16_t width, uint32_t height);
		static uint32_t getViewportTextureID(uint32_t ID);


	};

	class ApplicationAPI {
	public:

		static void createNewScene();


	};













}
