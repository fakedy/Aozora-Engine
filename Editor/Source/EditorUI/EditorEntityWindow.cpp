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
		m_registry->emplace<Aozora::TransformComponent>(entity);
		m_registry->emplace<Aozora::ModelComponent>(entity);
		
	}

	int i = 0;
	for (const auto entity : view) {
		auto& nameComponent = view.get<Aozora::NameComponent>(entity);
		std::string blabla = nameComponent.name + std::to_string(i);

		if (ImGui::Selectable(blabla.c_str())) {
			m_componentsView->setSelectedEntity(entity);
		}
		i++;
	}

	ImGui::End();
}
