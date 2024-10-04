#pragma once 
#include "Engine.h"
#include "Systems/ECS/Components/TransformComponent.h"
#include "Systems/ECS/Components/meshComponent.h"

class EditorEntityWindow {
public:

	EditorEntityWindow(std::shared_ptr<entt::registry> registry) {
		m_registry = registry;
	}

	void draw();

private:

	uint32_t selectedObject;
	std::shared_ptr<entt::registry> m_registry;

};