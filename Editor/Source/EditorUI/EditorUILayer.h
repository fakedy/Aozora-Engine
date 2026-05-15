#pragma once
#include "imgui/imgui.h"
#include "EditorLayer.h"
#include <memory>

namespace Aozora {
	class Context;
}

class EditorLayer;
class EditorEntityWindow;
class ComponentsView;
class StatsView;
class Workspace;

class EditorUILayer : public Aozora::Layer {
public:

	EditorUILayer(EditorLayer* editlayer, Aozora::Context& context);
	void onUpdate(const Aozora::Context& context) override;
	void onAttach() override;


private:
	Aozora::Context& m_context;

	std::shared_ptr<EditorEntityWindow> m_editorEntityWindow;
	std::shared_ptr<ComponentsView> m_componentsViewWindow;
	std::shared_ptr<StatsView> m_statsViewWindow;
	std::shared_ptr<Workspace> m_workspace;

	ImTextureID m_file_3d_texture;
	ImTextureID m_image_texture;
	ImTextureID m_folder_texture;

	EditorLayer* m_editorLayer;

	void setUIStyle();

};