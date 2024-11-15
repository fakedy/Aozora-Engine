#include "ComponentsView.h"



void ComponentsView::draw() {

	ImGui::Begin("Components View"); // will display components



	ImGui::End();
}

void ComponentsView::setSelectedEntity(entt::entity entity)
{
	m_selectedEntity = entity;


}
