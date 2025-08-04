#pragma once
#include <Systems/SceneManager/SceneManager.h>
#include <Systems/ScriptManager/ScriptSystem.h>
#include "Systems/ResourceManager/ResourceManager.h"
#include "Systems/Scene/Scene.h"
#include "Systems/CameraSystem.h"
#include "Systems/Renderers/SceneRenderer.h"
#include "Systems/Renderers/IrenderAPI.h"

namespace Aozora {

	class Context {
	public:
        SceneManager* sceneManager{ nullptr };
        IrenderAPI* renderAPI{ nullptr };
        SceneRenderer* sceneRenderer{ nullptr };
        ScriptSystem* scriptSystem{ nullptr };
	};
}
