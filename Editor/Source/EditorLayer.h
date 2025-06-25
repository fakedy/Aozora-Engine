#pragma once
#include "AozoraAPI/Aozora.h"
#include <Systems/EditorCameraSystem.h>
#include <memory>




enum class EditorState { EDIT, PLAY };



class EditorLayer : public Aozora::Layer
{
public:
	EditorLayer() {

		m_editorCameraSystem = std::make_unique<Aozora::EditorCameraSystem>();
	}

	void onUpdate() override;


private:


	EditorState state{ EditorState::EDIT };

	std::unique_ptr<Aozora::EditorCameraSystem> m_editorCameraSystem;

};

