#pragma once
#include "AozoraAPI/Aozora.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glad/glad.h"
#include <iostream>
#include "EditorEntityWindow.h"
#include "ComponentsView.h"
#include "StatsView.h"
#include "Systems/Renderers/Viewport.h"
#include <Systems/Scene/Scene.h>

class EditorUILayer : public Aozora::Layer {
public:
	EditorUILayer(){

		Aozora::Scene& scene = Aozora::Application::getApplication().getCurrentScene();

		m_componentsViewWindow = std::make_shared<ComponentsView>();
		m_editorEntityWindow = std::make_shared<EditorEntityWindow>(m_componentsViewWindow);

		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();


		// based on 4k for now. should use 1.0f for 1440p or something
		float scaleFactor = 1.5f;

		ImGuiStyle& style = ImGui::GetStyle();
		style.ScaleAllSizes(scaleFactor);

		io.Fonts->AddFontFromFileTTF("Resources/moon_get-Heavy.ttf", 15.0f * scaleFactor);


		// use api?
		Aozora::Application& m_app = Aozora::Application::getApplication();
		GLFWwindow* m_window = static_cast<GLFWwindow*>(m_app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(m_window, true);
		ImGui_ImplOpenGL3_Init();


		m_editorViewPortID = Aozora::RenderAPI::createViewport(&scene, Aozora::ViewportType::PrimaryEditor);
		m_gameViewPortID = Aozora::RenderAPI::createViewport(&scene, Aozora::ViewportType::PrimaryGame);

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

	uint32_t m_gameViewPortID;
	uint32_t m_editorViewPortID;

	std::shared_ptr<EditorCamera> m_editorCamera;

};