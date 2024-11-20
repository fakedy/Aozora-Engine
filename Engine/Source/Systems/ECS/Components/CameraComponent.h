#pragma once
#include <memory>
#include "Systems/Renderers/Camera.h"
namespace Aozora {

	struct CameraComponent {

		std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	};


}