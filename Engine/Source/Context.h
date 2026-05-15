#pragma once

namespace Aozora {

    class SceneManager;
    class IrenderAPI;
    class ScriptSystem;
    class ResourceManager;
    class ProjectManager;
    class CommandQueue;
    class EditorCameraSystem;
    class CameraSystem;
    class Window;

    namespace Graphics {
        class SceneRenderer;
    }

    namespace Resources {
        class AssetManager;
    }

	class Context {
	public:
        SceneManager* sceneManager{ nullptr };
        IrenderAPI* renderAPI{ nullptr };
        Graphics::SceneRenderer* sceneRenderer{ nullptr };
        ScriptSystem* scriptSystem{ nullptr };
        ResourceManager* resourcemanager{ nullptr };
        ProjectManager* projectManager{ nullptr };
        Resources::AssetManager* assetManager{ nullptr };
        CommandQueue* commandQueue{ nullptr };
        EditorCameraSystem* editorCameraSystem{ nullptr };
        CameraSystem* cameraSystem{ nullptr };
        Window* window{ nullptr };
	};
}
