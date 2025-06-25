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

	std::vector<entt::entity> view = Aozora::SceneAPI::getSceneHierarchyEntities();

	uint32_t entityCount = 0;
	for (entt::entity entity: view) {
		std::string entityName = Aozora::SceneAPI::getEntityName(entity) + std::to_string(entityCount);
		// if component also have children we need to figure that out :)
		if (ImGui::Selectable(entityName.c_str())) {
			m_componentsView->setSelectedEntity(entity);
		}
		entityCount++;
	}

	ImGui::End();
}
