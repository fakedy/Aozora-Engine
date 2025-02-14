#include "EditorUILayer.h"

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
	ImGui::Begin("Editor", NULL); // textureid
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	m_editorCamera->m_viewPortX = contentRegion.x;
	m_editorCamera->m_viewPortY = contentRegion.y;
	m_editorCamera->calcProjection();
	ImGui::Image((void*)(intptr_t)m_editorFramebuffer->m_colorTextureID, ImVec2(contentRegion.x, contentRegion.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


	// game window
	ImGui::Begin("Game", NULL); // textureid
	ImVec2 contentRegionGame = ImGui::GetContentRegionAvail();
	if (m_currentScene->m_activeCamera != nullptr) {
		m_currentScene->m_activeCamera->m_viewPortX = contentRegionGame.x;
		m_currentScene->m_activeCamera->m_viewPortY = contentRegionGame.y;
	}
	ImGui::Image((void*)(intptr_t)m_gameFramebuffer->m_colorTextureID, ImVec2(contentRegionGame.x, contentRegionGame.y), ImVec2(0, 1), ImVec2(1, 0));
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
	m_editorFramebuffer->unbind();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	m_editorFramebuffer->bind();
	m_currentScene->editorUpdate(m_editorCamera);
	m_editorFramebuffer->unbind();

	m_gameFramebuffer->bind();
	m_currentScene->renderScene();
	m_gameFramebuffer->unbind();

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

void EditorUILayer::resizeFramebuffer(int x, int y)
{
	/*
	// move to framebuffer
	m_framebufferSizeX = x;
	m_framebufferSizeY = y;
	glBindTexture(GL_TEXTURE_2D, m_editColorTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_framebufferSizeX, m_framebufferSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, m_editDepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_framebufferSizeX, m_framebufferSizeY, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_editorFramebuffer.get()->bufferTexture(m_editColorTextureID, m_editDepthTextureID);
	std::cout << "x: " << x << " y: " << y << std::endl;
	*/
}
