#include "EditorEntityWindow.h"
#include "imgui/imgui.h"
#include "entt/entt.hpp"
#include <string>

void EditorEntityWindow::draw(const Aozora::Context& context)
{
	ImGui::Begin("SceneGraph");

	if (ImGui::Button("Create entity")) {

		context.sceneManager->getCurrentActiveScene()->addEntity();

	}

	const std::vector<entt::entity>& view = context.sceneManager->getCurrentActiveScene()->getSceneHierarchyEntities();

	for (entt::entity entity: view) {
		// only draw root nodes, the children will get drawn from them
		if (context.sceneManager->getCurrentActiveScene()->getEntityParent(entity) == entt::null) {
			drawEntityNode(entity, context);
		}
	}

	ImGui::End();
}

void EditorEntityWindow::drawEntityNode(entt::entity entity, const Aozora::Context& context)
{

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;

	const auto& children = context.sceneManager->getCurrentActiveScene()->getEntityChildren(entity);
	std::string entityName = context.sceneManager->getCurrentActiveScene()->getEntityName(entity);

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
			drawEntityNode(child, context);
		}
		ImGui::TreePop();
	}


}
