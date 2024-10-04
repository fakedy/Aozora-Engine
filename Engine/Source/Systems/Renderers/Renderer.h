#pragma once
#include "entt/entt.hpp"

namespace Aozora {

	class Renderer
	{
	public:


		virtual void render() = 0;

		static Renderer* create(std::shared_ptr<entt::registry> registry);

	private:
		

	};
}
