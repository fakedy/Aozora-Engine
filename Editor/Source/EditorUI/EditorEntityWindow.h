#pragma once 
#include "AozoraAPI/Aozora.h"
#include "ComponentsView.h"
#include "Systems/ResourceManager/ResourceManager.h"
#include <Context.h>

class EditorEntityWindow {
public:
	EditorEntityWindow(std::shared_ptr<ComponentsView> componentsView) {
		m_componentsView = componentsView;
	}

	void draw(const Aozora::Context& context);

	void drawEntityNode(entt::entity entity);

private:
	std::shared_ptr<ComponentsView> m_componentsView;
};