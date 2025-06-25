#pragma once 
#include "AozoraAPI/Aozora.h"
#include "Systems/ECS/Components/TransformComponent.h"

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