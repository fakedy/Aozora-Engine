#include "EditorUILayer.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "EditorEntityWindow.h"


void EditorUILayer::onUpdate(){

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

	// editor window
	ImGui::Begin("Editor", NULL); // textureid
	ImGui::Image((void*)(intptr_t)0, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	// game window
	ImGui::Begin("Game", NULL); // textureid
	ImGui::Image((void*)(intptr_t)0, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();

	sceneGraph(); // the list of entities in the scene

	ImGui::Begin("workspace");
	ImGui::End();

	ImGui::Begin("properties");
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


void EditorUILayer::sceneGraph()
{
	EditorEntityWindow window(m_registry);
	window.draw();
	
}
