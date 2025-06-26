#include "ComponentsView.h"
#include "Systems/ECS/Components/NameComponent.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/CameraComponent.h"
#include "Systems/ECS/Components/RigidBodyComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"
#include "glm/gtc/type_ptr.hpp"
#include <Application.h>


ComponentsView::ComponentsView()
{

}

void ComponentsView::draw() {

	
	entt::registry& registry = Aozora::Application::getApplication().getCurrentScene().getRegistry();

	auto view = registry.view<Aozora::NameComponent>();
	ImGui::Begin("Components View", NULL, ImGuiWindowFlags_MenuBar); // will display components

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("New")) {
				if (ImGui::BeginMenu("Component")) {
					if (ImGui::MenuItem("Camera")) {
						registry.emplace_or_replace<Aozora::CameraComponent>(m_selectedEntity);
					}
					if (ImGui::MenuItem("Mesh")) {
						registry.emplace_or_replace<Aozora::MeshComponent>(m_selectedEntity);
					}
					if (ImGui::MenuItem("RigidBody")) {
						registry.emplace_or_replace<Aozora::RigidBodyComponent>(m_selectedEntity);
					}
					if (ImGui::MenuItem("Script")) {

					}
				
					ImGui::EndMenu();
				}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (m_selectedEntity != entt::null && registry.valid(m_selectedEntity)) {


		if (registry.all_of<Aozora::TransformComponent>(m_selectedEntity)) {
			auto& transformComp = registry.get<Aozora::TransformComponent>(m_selectedEntity);
			ImGui::Text("Transform component");
			ImGui::DragFloat3("Transform", glm::value_ptr(transformComp.pos), 0.1f);
			ImGui::DragFloat3("Scale", glm::value_ptr(transformComp.scale), 0.1f);
			ImGui::DragFloat3("Rotation", glm::value_ptr(transformComp.rot), 0.1f);
		}

		if (registry.all_of<Aozora::CameraComponent>(m_selectedEntity)) {
			auto& cameraComp = registry.get<Aozora::CameraComponent>(m_selectedEntity);
			ImGui::Text("Camera component");
		}

		if (registry.all_of<Aozora::MeshComponent>(m_selectedEntity)) {
			auto& meshComp = registry.get<Aozora::MeshComponent>(m_selectedEntity);
			ImGui::Text("Mesh component");
			ImGui::Text("Mesh ID: ", meshComp.meshID);
			ImGui::Text("Material ID: ", meshComp.material); // TODO doesnt work
			                                                                                                          
		}

	}
	ImGui::End();
}

void ComponentsView::setSelectedEntity(entt::entity entity)
{
	m_selectedEntity = entity;
}
