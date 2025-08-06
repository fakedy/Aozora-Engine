#pragma once
#include "Systems/Scene/Scene.h"
#include <entt/entt.hpp>
#include "IrenderAPI.h"
#include <Systems/ECS/Components/Components.h>
#include <functional>

namespace Aozora {

	class IrenderPipeline {


	public:



		virtual void execute(IrenderAPI& renderAPI, Scene& scene, entt::entity camera, uint32_t width, uint32_t height) = 0;

		virtual uint32_t getFinalImage() = 0;

		virtual void resize(uint32_t width, uint32_t height) = 0;

		virtual void genMegaBuffer(Scene& scene) = 0;
		virtual void updateMegaBuffer(Scene& scene) = 0;

		struct PSO {
		public:
			// pipeline state object
			/*
			* Role of the PSO is to set the necessary states before drawing.
			*/

			std::function<void()> stateCommands;

			void bind() {
				// set the necessary states


				stateCommands();

			}
		};

		struct DrawElementsIndirectCommand {
			unsigned int count{ 0 };
			unsigned int instanceCount{ 0 };
			unsigned int firstIndex{ 0 };
			int baseVertex{ 0 };
			unsigned int baseInstance{ 0 };
		};

	};
}