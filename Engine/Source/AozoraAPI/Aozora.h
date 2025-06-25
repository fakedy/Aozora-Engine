#pragma once
// Includes used by applications

#include "Application.h"
#include "Systems/Layers/ImguiLayer.h"
#include "Systems/Layers/Layer.h"
#include "Systems/Renderers/Viewport.h"


namespace Aozora {


	// api


	class SceneAPI {
	public:

		static void addEntity();
		static void deleteEntity();


	};

	class RenderAPI {
	public:

		static uint32_t createViewport(Scene* scene, ViewportType type);
		static void resizeViewport(uint32_t ID, uint16_t width, uint32_t height);
		static uint32_t getViewportTextureID(uint32_t ID);


	};

	class ApplicationAPI {
	public:

		static void createNewScene();


	};













}
