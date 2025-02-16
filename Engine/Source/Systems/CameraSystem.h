#pragma once
#include <memory>
#include <entt/entt.hpp>


namespace Aozora {

	class CameraSystem
	{
	public:

		CameraSystem(std::shared_ptr<entt::registry> registry);

		void update();



	private:


		std::shared_ptr<entt::registry> m_registry;

	};
}

