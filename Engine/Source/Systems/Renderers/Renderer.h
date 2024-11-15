#pragma once
#include "entt/entt.hpp"
#include "Systems/Windows/Window.h"
#include "Camera.h"
#include "Systems/Scene/Scene.h"

namespace Aozora {

	class Renderer
	{
	public:


		virtual void render() = 0;

		static Renderer* create(std::shared_ptr<Scene> scene, Window::WindowProps props);

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	private:

	protected:
		std::shared_ptr<entt::registry> m_registry;
		Window::WindowProps m_props;
		std::shared_ptr<Camera> m_camera;
		std::shared_ptr<Scene> m_scene;

	};
}
