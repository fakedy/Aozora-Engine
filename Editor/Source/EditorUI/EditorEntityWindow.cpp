#include "EditorEntityWindow.h"
#include "imgui/imgui.h"
#include "entt/entt.hpp"

void EditorEntityWindow::draw()
{

	ImGui::Begin("SceneGraph");
	// temp
	if (ImGui::Button("Create entity")) {
		m_registry->emplace<Aozora::Transform>(m_registry->create(), glm::vec3(0, 0, 0));
		m_registry->emplace<Aozora::meshComponent>(m_registry->create());
	}

	for (auto entity : m_registry->view<entt::entity>()) {
		ImGui::Text("An entity");
	}

	ImGui::End();
}
