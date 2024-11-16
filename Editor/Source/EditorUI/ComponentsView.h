#pragma once
#include <imgui/imgui.h>
#include "entt/entt.hpp"



class ComponentsView {
public:
	ComponentsView(std::shared_ptr<entt::registry> registry);
	void draw();
	void setSelectedEntity(entt::entity entity);
private:

	std::shared_ptr<entt::registry> m_registry;
	entt::entity m_selectedEntity;

};