#pragma once 
#include "Engine.h"
#include "ComponentsView.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/modelComponent.h"
#include "Systems/ECS/Components/NameComponent.h"


class EditorEntityWindow {
public:
	EditorEntityWindow(std::shared_ptr<entt::registry> registry, std::shared_ptr<ComponentsView> componentsView) {
		m_registry = registry;
		m_componentsView = componentsView;
	}

	void draw();

private:
	std::shared_ptr<entt::registry> m_registry;
	std::shared_ptr<ComponentsView> m_componentsView;
};