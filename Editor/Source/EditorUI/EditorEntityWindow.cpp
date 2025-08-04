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

	const std::vector<entt::entity>& view = Aozora::SceneAPI::getSceneHierarchyEntities();

	for (entt::entity entity: view) {
		// only draw root nodes, the children will get drawn from them
		if (Aozora::SceneAPI::getEntityParent(entity) == entt::null) {
			drawEntityNode(entity);
		}
	}

	ImGui::End();
}

void EditorEntityWindow::drawEntityNode(entt::entity entity)
{

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	const auto& children = Aozora::SceneAPI::getEntityChildren(entity);
	std::string entityName = Aozora::SceneAPI::getEntityName(entity);

	// check for entity children
	if (children.empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	if (m_componentsView->getSelectedEntity() == entity) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}


	bool nodeOpen = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, "%s", entityName.c_str());

	if (ImGui::IsItemClicked()) {
		m_componentsView->setSelectedEntity(entity);
	}
	// add right click remove


	if (nodeOpen) {

		for (entt::entity child : children) {
			drawEntityNode(child);
		}
		ImGui::TreePop();
	}


}
