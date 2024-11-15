#include "EditorEntityWindow.h"
#include "imgui/imgui.h"
#include "entt/entt.hpp"
#include <string>

void EditorEntityWindow::draw()
{
	auto view = m_registry->view<Aozora::NameComponent>();

	ImGui::Begin("SceneGraph");

	// temp
	if (ImGui::Button("Create entity")) {
		const auto entity = m_registry->create();
		m_registry->emplace<Aozora::NameComponent>(entity).name = "Entity";
		m_registry->emplace<Aozora::Transform>(entity);
		m_registry->emplace<Aozora::meshComponent>(entity);
	}

	for (const auto entity : view) {
		auto& nameComponent = view.get<Aozora::NameComponent>(entity);
		if (ImGui::Selectable(nameComponent.name)) {
			m_componentsView->setSelectedEntity(entity);
		}
		
	}

	ImGui::End();
}
