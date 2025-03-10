#include "EditorEntityWindow.h"
#include "imgui/imgui.h"
#include "entt/entt.hpp"
#include <string>

void EditorEntityWindow::draw()
{
	Aozora::ResourceManager& resourceManager = Aozora::ResourceManager::getResourceManager();
	auto view = m_registry->view<Aozora::NameComponent>();

	ImGui::Begin("SceneGraph");

	// temp
	if (ImGui::Button("Create entity")) {
		const auto entity = m_registry->create();
		m_registry->emplace<Aozora::NameComponent>(entity).name = "Entity";
		m_registry->emplace<Aozora::TagComponent>(entity);
		m_registry->emplace<Aozora::TransformComponent>(entity);
		m_registry->emplace<Aozora::RelationComponent>(entity);
		m_registry->emplace<Aozora::MeshComponent>(entity).meshIDs = resourceManager.loadModel("Resources/cube/cube.obj");
		//m_resourceManager.loadModel("Resources/cube/cube.obj");
		//m_resourceManager.loadModel("Resources/cube/cube.obj");
		//m_meshes = source.loadModel("Resources/sponza/sponza.obj");
		//m_meshes = source.loadModel("Resources/cube/cube.obj");
		//m_meshes = source.loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//m_meshes = source.loadModel("Resources/hintze-hall-vr-tour/source/hintze-hall_UV_pack01.fbx");
		//m_meshes = source.loadModel("Resources/survival-guitar-backpack/source/Survival_BackPack_2.fbx");
		//m_meshes = source.loadModel("Resources/cube/cube.obj");
		//m_resourceManager.loadModel("Resources/DamagedHelmet/DamagedHelmet.gltf");
		//m_meshes = source.loadModel("Resources/gpmesh/scene.gltf");

		
	}

	// TODO CLEAN
	for (int i = view.size(); i > 0; i--) {
		auto& nameComponent = view.get<Aozora::NameComponent>(view[i-1]);
		std::string entityName = nameComponent.name + std::to_string(view.size() - i);
		// if component also have children we need to figure that out :)
		if (ImGui::Selectable(entityName.c_str())) {
			m_componentsView->setSelectedEntity(view[i-1]);
		}
	}


	ImGui::End();
}
