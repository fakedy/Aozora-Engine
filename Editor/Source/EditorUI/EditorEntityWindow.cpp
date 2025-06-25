#include "EditorEntityWindow.h"
#include "imgui/imgui.h"
#include "entt/entt.hpp"
#include <string>
#include <AozoraAPI/Aozora.h>

void EditorEntityWindow::draw()
{
	ImGui::Begin("SceneGraph");

	if (ImGui::Button("Create entity")) {

		Aozora::SceneAPI::addEntity();

	}

	// wack
	/*
	auto view = m_registry->view<Aozora::NameComponent>();

	for (int i = view.size(); i > 0; i--) {
		auto& nameComponent = view.get<Aozora::NameComponent>(view[i-1]);
		std::string entityName = nameComponent.name + std::to_string(view.size() - i);
		// if component also have children we need to figure that out :)
		if (ImGui::Selectable(entityName.c_str())) {
			m_componentsView->setSelectedEntity(view[i-1]);
		}
	}
	*/

	ImGui::End();
}
