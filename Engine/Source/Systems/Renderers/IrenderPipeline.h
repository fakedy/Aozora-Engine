#pragma once
#include "Systems/Scene/Scene.h"
#include <entt/entt.hpp>
#include "IrenderAPI.h"
#include <Systems/ECS/Components/Components.h>


namespace Aozora {

	class IrenderPipeline {


	public:



		virtual void execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height) = 0;

		virtual uint32_t getFinalImage() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;



	};
}