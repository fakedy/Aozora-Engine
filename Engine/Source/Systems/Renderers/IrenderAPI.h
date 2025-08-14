#pragma once
#include <glm/glm.hpp>
#include "Shader.h"

namespace Aozora {

	class IrenderAPI
	{
	public:


		virtual void clear(float r, float g, float b, float a) = 0;

		static IrenderAPI* create();

		virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual uint32_t bakeCubemapIrradiance(uint32_t sourceID, uint32_t targetID) = 0;


	private:

	protected:

	};
}
