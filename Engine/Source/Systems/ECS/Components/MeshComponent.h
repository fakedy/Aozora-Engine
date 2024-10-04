#pragma once
#include <memory>
#include "Systems/Mesh.h"

namespace Aozora {

	struct meshComponent {
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
	};

}