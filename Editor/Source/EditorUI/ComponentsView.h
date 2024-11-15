#pragma once
#include <imgui/imgui.h>
#include "entt/entt.hpp"



class ComponentsView {
public:
	void draw();
	void setSelectedEntity(entt::entity entity);
private:

	entt::entity m_selectedEntity;

};