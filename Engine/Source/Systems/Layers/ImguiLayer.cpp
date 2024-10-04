#include "Systems/Layers/ImguiLayer.h"
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



namespace Aozora {

	ImguiLayer::ImguiLayer()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		ImGui::StyleColorsDark();

		Application& app = Application::getApplication();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.getWindow().getNativeWindow());


		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		
	}

	void ImguiLayer::onUpdate()
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));

		ImGui::SetNextWindowPos(ImVec2(0, 0));
		ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
		ImGui::Begin("DockSpace", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
		ImGuiID dockspace_id = ImGui::GetID("dockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
		ImGui::End();
		ImGui::PopStyleVar();

		ImGui::Begin("testing1");
		ImGui::End();

		ImGui::Begin("testing2");
		ImGui::End();

		ImGui::Begin("testing3");
		ImGui::End();


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	}
}
