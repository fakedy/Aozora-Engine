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
#include "EditorLayer.h"
#include "Workspace.h"
#include <Context.h>

class EditorUILayer : public Aozora::Layer {
public:
	EditorUILayer(EditorLayer* editlayer) : m_editorLayer(editlayer){


		m_componentsViewWindow = std::make_shared<ComponentsView>();
		m_editorEntityWindow = std::make_shared<EditorEntityWindow>(m_componentsViewWindow);
		m_workspace = std::make_shared<Workspace>();

		
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



		


	}

	void onUpdate(const Aozora::Context& context) override;


private:
	std::shared_ptr<EditorEntityWindow> m_editorEntityWindow;
	std::shared_ptr<ComponentsView> m_componentsViewWindow;
	std::shared_ptr<StatsView> m_statsViewWindow;
	std::shared_ptr<Workspace> m_workspace;

	ImTextureID m_file_3d_texture;
	ImTextureID m_image_texture;
	ImTextureID m_folder_texture;

	EditorLayer* m_editorLayer;


};