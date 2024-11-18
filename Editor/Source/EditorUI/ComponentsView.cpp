#include "ComponentsView.h"
#include "Systems/ECS/Components/NameComponent.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "glm/gtc/type_ptr.hpp"


ComponentsView::ComponentsView(std::shared_ptr<entt::registry> registry)
{
	m_registry = registry;

}

void ComponentsView::draw() {

	auto view = m_registry->view<Aozora::NameComponent>();
	ImGui::Begin("Components View"); // will display components
	if (m_selectedEntity != entt::null && m_registry->valid(m_selectedEntity)) {


		if (m_registry->all_of<Aozora::TransformComponent>(m_selectedEntity)) {

			auto& transformComp = m_registry->get<Aozora::TransformComponent>(m_selectedEntity);
			ImGui::DragFloat3("Transform", glm::value_ptr(transformComp.pos), 0.1f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transformComp.scale), 0.1f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(transformComp.rot), 0.1f);

		}
		
	}
	ImGui::End();
}

void ComponentsView::setSelectedEntity(entt::entity entity)
{
	m_selectedEntity = entity;


}
