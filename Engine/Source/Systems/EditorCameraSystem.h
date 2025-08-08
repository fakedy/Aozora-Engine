#pragma once
#include <memory>
#include <entt/entt.hpp>
#include <Systems/Events/EventSystem.h>
#include <Systems/Events/Events.h>


/*
* Basically this is a system that handle the editor camera logic
*/


namespace Aozora {

	class EditorCameraSystem
	{
	public:

		EditorCameraSystem();

		void update(entt::registry& registry);


	private:


		float lastX = 1920 / 2.0f;
		float lastY = 1080 / 2.0f;

		float yaw = -90.0f; // rotate
		float pitch = 0.0f; // up and down

		float sensitivity = 0.3f;


		float movspeed{ 10.0f };

		bool mouseHeld{ false };

		Scene* m_currentScene;

	};
}

