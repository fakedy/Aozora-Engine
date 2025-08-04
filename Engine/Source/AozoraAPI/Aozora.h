#pragma once
// Includes used by applications

#include "Application.h"
#include "Systems/Layers/ImguiLayer.h"
#include "Systems/Layers/Layer.h"
#include "Systems/Renderers/Viewport.h"
#include "entt/entt.hpp"
#include <string>
#include <Systems/Model.h>
#include <Systems/Mesh.h>
#include <Systems/Events/Event.h>
#include <Systems/Events/Events.h>

namespace Aozora {


	// api
	// GONNA EXECUTE THIS ENTIRE API, HATE IT


	class SceneAPI {
	public:

		static void addEntity();
		static void deleteEntity(const entt::entity entity);
		static std::vector<entt::entity> getSceneHierarchyEntities();
		static std::string getEntityName(entt::entity entity);
		static std::vector<entt::entity>& getEntityChildren(entt::entity entity);
		static entt::entity& getEntityParent(entt::entity entity);
		static void makeTransformDirty(entt::entity entity);
		static void takeSnapshot();
		static void loadSnapshot();


	};

	class RenderAPI {
	public:

		static uint32_t createViewport(ViewportType type, entt::entity editorCameraEntity);
		static uint32_t createViewport(ViewportType type);
		static void resizeViewport(uint32_t ID, uint16_t width, uint32_t height);
		static void setViewportActive(uint32_t ID, bool condition);
		static uint32_t getViewportTextureID(uint32_t ID);


	};

	class ApplicationAPI {
	public:

		static void newProject();
		static void loadProject();
		static void saveProject();
		static uint32_t createNewScene();
		static void loadScene(uint32_t ID);
		static Scene* getScene(uint32_t id);


	};

	class ResourcesAPI {
	public:

		static void loadModel(std::string path);
		static uint32_t loadCubemap(const std::vector<std::string>& paths);
		static uint32_t loadCubemap();
		static SkyboxTextures loadSkybox(const std::vector<std::string>& paths);
		static uint32_t instantiateModel(std::string name);
		static uint32_t loadTexture(std::string name, bool persistent);

		static std::vector<std::string> getLoadedModelNames();
		static Model* getModel(std::string name);

		static Material& getMaterial(uint32_t id);

	};













}
