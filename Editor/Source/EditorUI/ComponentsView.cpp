#include "ComponentsView.h"
#include "Systems/ECS/Components/Components.h"
#include "glm/gtc/type_ptr.hpp"
#include <Application.h>
#include <Systems/Material.h>


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
						registry.emplace_or_replace<Aozora::ScriptComponent>(m_selectedEntity);
					}
					if (ImGui::MenuItem("Light")) {
						registry.emplace_or_replace<Aozora::LightComponent>(m_selectedEntity);
					}
				
					ImGui::EndMenu();
				}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	if (m_selectedEntity != entt::null && registry.valid(m_selectedEntity)) {


		if (registry.all_of<Aozora::TransformComponent>(m_selectedEntity)) {
			if (ImGui::CollapsingHeader("TransformComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
			
				auto& transformComp = registry.get<Aozora::TransformComponent>(m_selectedEntity);
				ImGui::Text("Transform component");

				if (ImGui::DragFloat3("Transform", glm::value_ptr(transformComp.pos), 0.1f)) {
					Aozora::SceneAPI::makeTransformDirty(m_selectedEntity);
				}
				if (ImGui::DragFloat3("Scale", glm::value_ptr(transformComp.scale), 0.1f)) {
					Aozora::SceneAPI::makeTransformDirty(m_selectedEntity);
				}
				if (ImGui::DragFloat3("Rotation", glm::value_ptr(transformComp.rot), 0.1f)) {
					Aozora::SceneAPI::makeTransformDirty(m_selectedEntity);
				}
			}
		}

		// check if our entity got a camera
		if (registry.all_of<Aozora::CameraComponent>(m_selectedEntity)) {

			if (ImGui::CollapsingHeader("CameraComponent", ImGuiTreeNodeFlags_DefaultOpen)) {
				auto& cameraComp = registry.get<Aozora::CameraComponent>(m_selectedEntity);
				ImGui::Text("Camera component");
			}
		}

		// check if entity got mesh
		if (registry.all_of<Aozora::MeshComponent>(m_selectedEntity)) {
			if (ImGui::CollapsingHeader("MeshComponent", ImGuiTreeNodeFlags_DefaultOpen)) {

				auto& meshComp = registry.get<Aozora::MeshComponent>(m_selectedEntity);
				ImGui::Text("Mesh ID: %i", meshComp.meshID);


				Aozora::Material& mat = Aozora::ResourcesAPI::getMaterial(meshComp.materialID);
				ImGui::Text("Material name: %s", mat.name);
				ImGui::Text("Material ID: %i", meshComp.materialID);
				ImGui::DragFloat4("Albedo", glm::value_ptr(mat.baseColor), 0.05f);
				ImGui::DragFloat4("Emissive", glm::value_ptr(mat.emissive), 0.05f);
			}
		}

		if (registry.all_of<Aozora::LightComponent>(m_selectedEntity)) {
			if (ImGui::CollapsingHeader("LightComponent", ImGuiTreeNodeFlags_DefaultOpen)) {

				auto& lightComp = registry.get<Aozora::LightComponent>(m_selectedEntity);
				//ImGui::Text("Type: %s", lightComp.type);
				ImGui::DragFloat3("Color", glm::value_ptr(lightComp.color), 0.05f);
				ImGui::DragFloat("Radius", &lightComp.radius, 0.05f);
				ImGui::DragFloat("Linear", &lightComp.linear, 0.05f);
				ImGui::DragFloat("Quadratic", &lightComp.quadratic, 0.05f);
				ImGui::DragFloat("Power", &lightComp.power, 0.05f);
			}
		}
		if (registry.all_of<Aozora::ScriptComponent>(m_selectedEntity)) {
			if (ImGui::CollapsingHeader("ScriptComponent", ImGuiTreeNodeFlags_DefaultOpen)) {

				auto& scriptComp = registry.get<Aozora::ScriptComponent>(m_selectedEntity);
				ImGui::Text("Nothing here yet");
			}
		}

	}
	ImGui::End();
}

void ComponentsView::setSelectedEntity(entt::entity entity)
{
	m_selectedEntity = entity;
}

entt::entity ComponentsView::getSelectedEntity()
{
	return m_selectedEntity;
}
