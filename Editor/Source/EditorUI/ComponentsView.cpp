#include "ComponentsView.h"
#include "Systems/ECS/Components/Components.h"
#include "glm/gtc/type_ptr.hpp"
#include <Application.h>
#include <Systems/Material.h>


ComponentsView::ComponentsView()
{

}

void ComponentsView::draw(const Aozora::Context& context) {

	uint64_t sceneID = context.sceneManager->getCurrentActiveScene()->hash;
	Aozora::Scene& scene = *context.sceneManager->getCurrentActiveScene();
	entt::registry& registry = scene.getRegistry();

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

				if (ImGui::DragScalarN("Transform", ImGuiDataType_Double, glm::value_ptr(transformComp.pos), 3, 0.1f)) {
					context.sceneManager->getCurrentActiveScene()->makeTransformDirty(m_selectedEntity);
				}
				if (ImGui::DragScalarN("Scale", ImGuiDataType_Double, glm::value_ptr(transformComp.scale), 3, 0.1f)) {
					context.sceneManager->getCurrentActiveScene()->makeTransformDirty(m_selectedEntity);
				}
				if (ImGui::DragScalarN("Rotation", ImGuiDataType_Double, glm::value_ptr(transformComp.rot), 3, 0.1f)) {
					context.sceneManager->getCurrentActiveScene()->makeTransformDirty(m_selectedEntity);
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

		Aozora::ResourceManager::ResourceContainer& sceneContainer = context.resourcemanager->m_containerMap[scene.hash];

		// check if entity got mesh
		if (registry.all_of<Aozora::MeshComponent>(m_selectedEntity)) {
			if (ImGui::CollapsingHeader("MeshComponent", ImGuiTreeNodeFlags_DefaultOpen)) {

				auto& meshComp = registry.get<Aozora::MeshComponent>(m_selectedEntity);
				ImGui::Text("Mesh ID: %llX", meshComp.meshID);


				Aozora::Material& mat = context.resourcemanager->getMaterial(meshComp.materialID, sceneID);
				ImGui::Text("Material name: %s", mat.name);
				ImGui::Text("Material ID: %llX", meshComp.materialID);
				// not sure this is correct but lets see
				// I need to check how i actually did my refCount




				int thumbnailSize = ImGui::GetContentRegionAvail().x;

				if (mat.diffuseTexture) {
					if (ImGui::CollapsingHeader("DiffuseTexture", ImGuiTreeNodeFlags_DefaultOpen)) {
						auto gpuHandle = sceneContainer.m_loadedTextures[mat.diffuseTexture].hash;
						ImGui::Image((ImTextureID)gpuHandle, ImVec2((float)thumbnailSize, (float)thumbnailSize));
					}
				}
				else {
					ImGui::DragFloat4("Albedo", glm::value_ptr(mat.baseColor), 0.01f, 0.0f, 1.0f);
				}

				if (mat.roughnessTexture) {
					if (ImGui::CollapsingHeader("Roughness Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
						auto gpuHandle = sceneContainer.m_loadedTextures[mat.roughnessTexture].hash;
						ImGui::Image((ImTextureID)gpuHandle, ImVec2((float)thumbnailSize, (float)thumbnailSize));
					}
				}
				else {
					ImGui::DragFloat("Roughness", &mat.roughness, 0.01f, 0.0f, 1.0f);
				}

				if (mat.metallicTexture) {
					if (ImGui::CollapsingHeader("Metallic Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
						auto gpuHandle = sceneContainer.m_loadedTextures[mat.metallicTexture].hash;
						ImGui::Image((ImTextureID)gpuHandle, ImVec2((float)thumbnailSize, (float)thumbnailSize));
					}
				}
				else {
					ImGui::DragFloat("Metallic", &mat.metallic, 0.01f, 0.0f, 1.0f);
				}

				if (mat.emissiveTexture) {
					if (ImGui::CollapsingHeader("Emissive Texture", ImGuiTreeNodeFlags_DefaultOpen)) {
						auto gpuHandle = sceneContainer.m_loadedTextures[mat.emissiveTexture].hash;
						ImGui::Image((ImTextureID)gpuHandle, ImVec2((float)thumbnailSize, (float)thumbnailSize));
					}
				}
				else {
					ImGui::DragFloat4("Emissive", glm::value_ptr(mat.emissive), 0.01f, 0.0f, 1.0f);
				}

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
				ImGui::Text("Script: %s", scriptComp.name);
				ImGui::Text("Script ID: %i", scriptComp.scriptID); // will be able to select a script by filling this ID
				ImGui::Checkbox("isActive", &scriptComp.isActive);
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
