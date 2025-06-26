#pragma once
#include <imgui/imgui.h>
#include "entt/entt.hpp"
#include "Systems/ResourceManager/ResourceManager.h"



class ComponentsView {
public:
	ComponentsView();
	void draw();
	void setSelectedEntity(entt::entity entity);
	entt::entity getSelectedEntity();
private:

	entt::entity m_selectedEntity;

};