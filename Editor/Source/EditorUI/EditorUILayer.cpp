#include "EditorUILayer.h"
#include "Application.h"


void EditorUILayer::onUpdate(const Aozora::Context& context){

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

					// create pop up to prevent accidental project deletion
					Aozora::CreateProjectRequest* event = new Aozora::CreateProjectRequest();
					Aozora::EventDispatcher::dispatch(event);
					
					// have this dispatch an event or something that request a project change
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Open")) {
				if (ImGui::MenuItem("Project")) {
					Aozora::SaveProjectRequest* event = new Aozora::SaveProjectRequest();
					Aozora::EventDispatcher::dispatch(event);
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Save")) {
				if (ImGui::MenuItem("Project")) {
					context.assetManager->saveProject();
				}
				if (ImGui::MenuItem("Scene")) {
					context.assetManager->saveSceneToDisk(*context.sceneManager->getCurrentActiveScene());
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem("Build")) {
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
	if (ImGui::Begin("Editor", NULL)) {
		
		context.sceneRenderer->setViewportActive(m_editorLayer->m_editorViewPortID, true);
		ImVec2 contentRegion = ImGui::GetContentRegionAvail();
		context.sceneRenderer->resizeViewport(m_editorLayer->m_editorViewPortID, contentRegion.x, contentRegion.y);

		uint32_t editorTextureID = context.sceneRenderer->getViewportTextureID(m_editorLayer->m_editorViewPortID);

		ImGui::Image((void*)(intptr_t)editorTextureID, ImVec2(contentRegion.x, contentRegion.y), ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
	}
	else {
		context.sceneRenderer->setViewportActive(m_editorLayer->m_editorViewPortID, false);
		ImGui::End();
	}

	ImGui::PopStyleVar();

	m_editorEntityWindow->draw(context); // the list of entities in the scene

	m_workspace->draw(context);

	m_componentsViewWindow->draw(context);

	ImGui::Begin("Console");
	ImGui::End();

	m_statsViewWindow->draw(context);


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}



