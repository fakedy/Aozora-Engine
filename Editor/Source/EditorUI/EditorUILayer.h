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

class EditorUILayer : public Aozora::Layer {
public:
	EditorUILayer(std::shared_ptr<Scene> scene) : m_currentScene(scene) {

		m_currentScene = scene;
		m_componentsViewWindow = std::make_shared<ComponentsView>(scene->registry);
		m_editorEntityWindow = std::make_shared<EditorEntityWindow>(scene->registry, m_componentsViewWindow);

		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		Aozora::Application& app = Aozora::Application::getApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		m_framebuffer = std::make_shared<Aozora::OpenglFrameBuffer>();
		m_framebuffer.get()->create();
		createTextures();
		m_framebuffer.get()->bufferTexture(m_editColorTextureID, m_editDepthTextureID);
		


	}

	void onUpdate() override;


private:

	void createTextures();
	std::shared_ptr<EditorEntityWindow> m_editorEntityWindow;
	std::shared_ptr<ComponentsView> m_componentsViewWindow;
	void sceneGraph();
	void componentsView();
	std::shared_ptr<Scene> m_currentScene;
	unsigned int m_editColorTextureID;
	unsigned int m_editDepthTextureID;
	std::shared_ptr<Aozora::FrameBuffer> m_framebuffer;
	int m_framebufferSizeX = 1920;
	int m_framebufferSizeY = 1080;

	void resizeFramebuffer(int x, int y);

};