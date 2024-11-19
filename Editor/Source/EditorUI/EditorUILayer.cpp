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
	ImGuiID dockspace_id = ImGui::GetID("dockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0, 0), ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::End();



	// editor window
	ImGui::Begin("Editor", NULL); // textureid
	ImVec2 contentRegion = ImGui::GetContentRegionAvail();
	m_editorCamera->m_viewPortX = contentRegion.x;
	m_editorCamera->m_viewPortY = contentRegion.y;
	m_editorCamera->calcProjection();
	ImGui::Image((void*)(intptr_t)m_editColorTextureID, ImVec2(contentRegion.x, contentRegion.y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();


	// game window
	ImGui::Begin("Game", NULL); // textureid
	ImGui::Image((void*)(intptr_t)m_editColorTextureID, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	ImGui::PopStyleVar();

	sceneGraph(); // the list of entities in the scene

	ImGui::Begin("workspace"); // file browser
	ImGui::End();

	componentsView();

	ImGui::Begin("Console");
	ImGui::End();


	// temporary really. right now we have to unbind because we default to rendering to framebuffer
	m_framebuffer.get()->unbind();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	m_framebuffer.get()->bind();
	m_currentScene->editorUpdate(m_editorCamera);

}

void EditorUILayer::createTextures()
{ 
	// dumb idea
	// create textures for the editor/game window
	// move to framebuffer

	glGenTextures(1, &m_editColorTextureID);
	glBindTexture(GL_TEXTURE_2D, m_editColorTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_framebufferSizeX, m_framebufferSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glGenTextures(1, &m_editDepthTextureID);
	glBindTexture(GL_TEXTURE_2D, m_editDepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_framebufferSizeX, m_framebufferSizeY, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

}




void EditorUILayer::sceneGraph()
{
	m_editorEntityWindow->draw();
}

void EditorUILayer::componentsView()
{
	m_componentsViewWindow->draw();
}

void EditorUILayer::resizeFramebuffer(int x, int y)
{
	// move to framebuffer
	m_framebufferSizeX = x;
	m_framebufferSizeY = y;
	glBindTexture(GL_TEXTURE_2D, m_editColorTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_framebufferSizeX, m_framebufferSizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	glBindTexture(GL_TEXTURE_2D, m_editDepthTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_framebufferSizeX, m_framebufferSizeY, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
	m_framebuffer.get()->bufferTexture(m_editColorTextureID, m_editDepthTextureID);
	std::cout << "x: " << x << " y: " << y << std::endl;
}
