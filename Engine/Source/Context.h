#pragma once
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/ScriptManager/ScriptSystem.h>
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/AssetManager/AssetManager.h"
#include "Systems/Scene/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/Renderers/SceneRenderer.h"
#include "Systems/Renderers/IrenderAPI.h"
#include "Systems/Project/ProjectManager.h"

namespace Aozora {

	class Context {
	public:
        SceneManager* sceneManager{ nullptr };
        IrenderAPI* renderAPI{ nullptr };
        Graphics::SceneRenderer* sceneRenderer{ nullptr };
        ScriptSystem* scriptSystem{ nullptr };
        ResourceManager* resourcemanager{ nullptr };
        ProjectManager* projectManager{ nullptr };
        Resources::AssetManager* assetManager{ nullptr };
	};
}
