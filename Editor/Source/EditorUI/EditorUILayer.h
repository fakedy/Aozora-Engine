#pragma once
#include "Engine.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


class EditorUILayer : public Aozora::Layer {
public:
	EditorUILayer(std::shared_ptr<entt::registry> registry) {

		m_registry = registry;
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		Aozora::Application& app = Aozora::Application::getApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void onUpdate() override;

private:

	void sceneGraph();
	std::shared_ptr<entt::registry> m_registry;



};