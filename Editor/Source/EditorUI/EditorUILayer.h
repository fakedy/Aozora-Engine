#pragma once
#include "Engine.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>
#include "EditorEntityWindow.h"
#include "ComponentsView.h"
#include "StatsView.h"
#include "Systems/Renderers/EditorCamera.h"
#include "Systems/ResourceManager/ResourceManager.h"

class EditorUILayer : public Aozora::Layer {
public:
	EditorUILayer(std::shared_ptr<Aozora::Scene> scene) : m_currentScene(scene) {

		m_currentScene = scene;
		m_componentsViewWindow = std::make_shared<ComponentsView>(scene->m_registry);
		m_editorEntityWindow = std::make_shared<EditorEntityWindow>(scene->m_registry, m_componentsViewWindow);

		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		Aozora::Application& m_app = Aozora::Application::getApplication();
		GLFWwindow* m_window = static_cast<GLFWwindow*>(m_app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init();

		m_editorFramebuffer = std::make_shared<Aozora::OpenglFrameBuffer>();
		m_editorFramebuffer.get()->create();
		m_editorFramebuffer.get()->createTextures(m_framebufferSizeX, m_framebufferSizeY);
		m_editorFramebuffer.get()->bufferTexture();

		m_gameFramebuffer = std::make_shared<Aozora::OpenglFrameBuffer>();
		m_gameFramebuffer.get()->create();
		m_gameFramebuffer.get()->createTextures(m_framebufferSizeX, m_framebufferSizeY);
		m_gameFramebuffer.get()->bufferTexture();

		m_editorCamera = std::make_shared<EditorCamera>();
		


	}

	void onUpdate() override;


private:
	std::shared_ptr<EditorEntityWindow> m_editorEntityWindow;
	std::shared_ptr<ComponentsView> m_componentsViewWindow;
	std::shared_ptr<StatsView> m_statsViewWindow;
	void sceneGraph();
	void componentsView();
	void statsView();
	std::shared_ptr<Aozora::Scene> m_currentScene;

	std::shared_ptr<Aozora::FrameBuffer> m_editorFramebuffer;
	std::shared_ptr<Aozora::FrameBuffer> m_gameFramebuffer;
	int m_framebufferSizeX = 1920;
	int m_framebufferSizeY = 1080;
	std::shared_ptr<EditorCamera> m_editorCamera;

	void resizeFramebuffer(int x, int y);

};