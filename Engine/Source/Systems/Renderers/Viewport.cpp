#include "Viewport.h"
#include "Opengl/OpenglFrameBuffer.h"


namespace Aozora {

	Viewport::Viewport(uint32_t width, uint32_t height, std::unique_ptr<IrenderPipeline> pipeline) : width(width), height(height), renderPipeline(std::move(pipeline))
	{
	}

	void Viewport::resize(uint32_t newWidth, uint32_t newHeight)
	{

		if (width != newWidth || height != newHeight) {

			width = width;
			height = height;
			//updateTexture(width, height);
		}


	}
}
