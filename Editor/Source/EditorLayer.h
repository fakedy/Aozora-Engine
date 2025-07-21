#pragma once
#include "AozoraAPI/Aozora.h"
#include <Systems/EditorCameraSystem.h>
#include <memory>
#include <entt/entt.hpp>
#include <Systems/ECS/Components/Components.h>
#include <Systems/Scene/Scene.h>
#include <Systems/Project/Project.h>


enum class EditorState { EDIT, PLAY };



class EditorLayer : public Aozora::Layer
{
public:
	EditorLayer() {

		Aozora::ApplicationAPI::newProject();


		m_editorViewPortID = Aozora::RenderAPI::createViewport(Aozora::ViewportType::PrimaryEditor);
		m_gameViewPortID = Aozora::RenderAPI::createViewport(Aozora::ViewportType::PrimaryGame);
		m_editorCameraSystem = std::make_unique<Aozora::EditorCameraSystem>();

	}

	void onUpdate() override;

	uint32_t m_gameViewPortID;
	uint32_t m_editorViewPortID;

private:

	EditorState state{ EditorState::EDIT };

	std::unique_ptr<Aozora::EditorCameraSystem> m_editorCameraSystem;



};

