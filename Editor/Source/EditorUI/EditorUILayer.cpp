#include "EditorUILayer.h"
#include "Application.h"

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


	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
				if (ImGui::MenuItem("Project")) {
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Open")) {
				if (ImGui::MenuItem("Project")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	ImGuiID dockspace_id = ImGui::GetID("dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();



	// editor window
	ImGui::Begin("Editor", NULL);
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	Aozora::RenderAPI::resizeViewport(m_editorViewPortID, contentRegion.x, contentRegion.y);


	uint32_t editorTextureID = Aozora::RenderAPI::getViewportTextureID(m_editorViewPortID);

	ImGui::Image((void*)(intptr_t)editorTextureID, ImVec2(contentRegion.x, contentRegion.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();



	// game window
	ImGui::Begin("Game", NULL);
	// get the imgui space
	ImVec2 contentRegionGame = ImGui::GetContentRegionAvail();
	Aozora::RenderAPI::resizeViewport(m_gameViewPortID, contentRegionGame.x, contentRegionGame.y);

	uint32_t gameTextureID = Aozora::RenderAPI::getViewportTextureID(m_gameViewPortID);

	ImGui::Image((void*)(intptr_t)gameTextureID, ImVec2(contentRegionGame.x, contentRegionGame.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();

	sceneGraph(); // the list of entities in the scene

	ImGui::Begin("workspace"); // file browser
	ImGui::End();

	componentsView();

	ImGui::Begin("Console");
	ImGui::End();

	statsView();


	// temporary really. right now we have to unbind because we default to rendering to framebuffer
	//m_editorFramebuffer->unbind();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}


void EditorUILayer::sceneGraph()
{
	m_editorEntityWindow->draw();
}

void EditorUILayer::componentsView()
{
	m_componentsViewWindow->draw();
}

void EditorUILayer::statsView()
{
	m_statsViewWindow->draw();
}


