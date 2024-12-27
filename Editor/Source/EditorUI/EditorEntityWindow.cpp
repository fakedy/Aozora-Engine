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

	for (int i = view.size(); i > 0; i--) {
		auto& nameComponent = view.get<Aozora::NameComponent>(view[i-1]);
		std::string blabla = nameComponent.name + std::to_string(view.size() - i);

		if (ImGui::Selectable(blabla.c_str())) {
			m_componentsView->setSelectedEntity(view[i-1]);
		}
	}

	ImGui::End();
}
