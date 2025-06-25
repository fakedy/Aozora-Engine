#pragma once 
#include "AozoraAPI/Aozora.h"
#include "ComponentsView.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/NameComponent.h"
#include "Systems/ECS/Components/RelationComponent.h"
#include "Systems/ECS/Components/MeshComponent.h"
#include "Systems/ECS/Components/TagComponent.h"
#include "Systems/ResourceManager/ResourceManager.h"


class EditorEntityWindow {
public:
	EditorEntityWindow(std::shared_ptr<ComponentsView> componentsView) {
		m_componentsView = componentsView;
	}

	void draw();

private:
	std::shared_ptr<ComponentsView> m_componentsView;
};