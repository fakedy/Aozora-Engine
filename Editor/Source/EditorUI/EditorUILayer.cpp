#include "EditorUILayer.h"
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
#include "Workspace.h"
#include <Context.h>
#include <Systems/Logging/Logger.h>


EditorUILayer::EditorUILayer(EditorLayer* editlayer, Aozora::Context& context) : m_editorLayer(editlayer), m_context(context)
{

	m_componentsViewWindow = std::make_shared<ComponentsView>();
	m_editorEntityWindow = std::make_shared<EditorEntityWindow>(m_componentsViewWindow);
	m_workspace = std::make_shared<Workspace>(m_context);


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

	Aozora::Window& window = *m_context.window;
	GLFWwindow* m_window = static_cast<GLFWwindow*>(window.getNativeWindow());

	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init();
}

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
					// have this dispatch an event or something that request a project change
					Aozora::CreateProjectRequest* event = new Aozora::CreateProjectRequest();
					Aozora::EventDispatcher::dispatch(event);
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


	ImGui::BeginChild("Scrolling");

	for (const auto& log : Aozora::Log::textLog) {

		switch (log.level)
		{
		case Aozora::Log::Info:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, 1.0));
			break;
		case Aozora::Log::Warning:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0,1.0,0.0,1.0));
			break;
		case Aozora::Log::Error:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 0.0, 0.0, 1.0));
			break;
		default:
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0, 1.0, 1.0, 1.0));
			break;
		}
		ImGui::TextUnformatted(log.text.c_str());
		ImGui::PopStyleColor();
	}

	if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) {
		ImGui::SetScrollHereY(1.0f);
	}


	ImGui::EndChild();

	ImGui::End();

	m_statsViewWindow->draw(context);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EditorUILayer::onAttach()
{
	setUIStyle();
}

void EditorUILayer::setUIStyle()
{
	// setup the theme, would be cool to have preset themes to be selectable
	ImGuiStyle& style = ImGui::GetStyle();
	ImGuiIO& io = ImGui::GetIO();

	style.WindowRounding = 4.0f;
	style.FrameRounding = 2.0f;
	style.ScrollbarRounding = 2.0f;
	style.GrabRounding = 2.0f;
	style.TabRounding = 2.0f;

	style.WindowBorderSize = 1.0f;
	style.FrameBorderSize = 0.0f;
	style.PopupBorderSize = 1.0f;

	ImVec4* colors = style.Colors;

	// Text
	colors[ImGuiCol_Text] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.55f, 0.65f, 1.00f);

	// Backgrounds
	colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.16f, 0.21f, 1.00f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.15f, 0.18f, 0.24f, 1.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.11f, 0.13f, 0.18f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.25f, 0.35f, 0.45f, 1.00f);

	// Inputs & Frames
	colors[ImGuiCol_FrameBg] = ImVec4(0.18f, 0.22f, 0.29f, 1.00f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.30f, 0.40f, 1.00f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.29f, 0.56f, 0.89f, 0.50f);

	// Top Bars
	colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.12f, 0.16f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.15f, 0.22f, 0.32f, 1.00f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.12f, 0.16f, 1.00f);

	// Accents
	colors[ImGuiCol_Button] = ImVec4(0.22f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.55f, 0.85f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.15f, 0.35f, 0.60f, 1.00f);

	colors[ImGuiCol_Header] = ImVec4(0.22f, 0.45f, 0.70f, 0.80f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.30f, 0.55f, 0.85f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.15f, 0.35f, 0.60f, 0.80f);

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4(0.15f, 0.20f, 0.28f, 1.00f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.30f, 0.55f, 0.85f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.22f, 0.45f, 0.70f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.13f, 0.16f, 0.21f, 1.00f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.18f, 0.25f, 0.35f, 1.00f);

	// Misc Elements
	colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.90f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.30f, 0.55f, 0.85f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.40f, 0.65f, 0.95f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.25f, 0.35f, 0.45f, 1.00f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.30f, 0.55f, 0.85f, 1.00f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.65f, 0.95f, 1.00f);
}



