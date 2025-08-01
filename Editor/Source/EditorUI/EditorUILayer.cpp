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

					// NEW WOOOOOORLD
					Aozora::ApplicationAPI::newProject();
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

		// temp location
		if (m_editorLayer->getState() == EditorState::EDIT) {

			if (ImGui::Button("Play")) {
				m_editorLayer->changeState(EditorState::PLAY);
			}
		}
		else {

			if (ImGui::Button("Stop")) {
				m_editorLayer->changeState(EditorState::EDIT);
			}
		}

		ImGui::EndMenuBar();
	}

	ImGuiID dockspace_id = ImGui::GetID("dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();



	// editor window
	ImGui::Begin("Editor", NULL);
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	Aozora::RenderAPI::resizeViewport(m_editorLayer->m_editorViewPortID, contentRegion.x, contentRegion.y);


	uint32_t editorTextureID = Aozora::RenderAPI::getViewportTextureID(m_editorLayer->m_editorViewPortID);
	ImGui::Image((void*)(intptr_t)editorTextureID, ImVec2(contentRegion.x, contentRegion.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();



	// game window
	ImGui::Begin("Game", NULL);
	// get the imgui space
	ImVec2 contentRegionGame = ImGui::GetContentRegionAvail();
	Aozora::RenderAPI::resizeViewport(m_editorLayer->m_gameViewPortID, contentRegionGame.x, contentRegionGame.y);

	uint32_t gameTextureID = Aozora::RenderAPI::getViewportTextureID(m_editorLayer->m_gameViewPortID);

	ImGui::Image((void*)(intptr_t)gameTextureID, ImVec2(contentRegionGame.x, contentRegionGame.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();

	sceneGraph(); // the list of entities in the scene

	ImGui::Begin("workspace"); // file browser





	std::vector <std::string> loadedModelNames = Aozora::ResourcesAPI::getLoadedModelNames();

	for (std::string modelName : loadedModelNames) {
		if (ImGui::Selectable(modelName.c_str())) {
			Aozora::ResourcesAPI::instantiateModel(modelName);
		}

		// not working for now
		// ImGui::ImageButtonEx(0,0, ImVec2(32,32), ImVec2(0, 0), ImVec2(1, 1), ImVec4(1,1,1,1), ImVec4(0, 1, 1, 1));
	}




	ImGui::End();

	componentsView();

	ImGui::Begin("Console");
	ImGui::End();

	statsView();


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


