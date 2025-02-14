#pragma once
#include <memory>
#include "Systems/Model.h"
#include "Systems/Material.h"

namespace Aozora {

	struct ModelComponent {
		std::unique_ptr<Model> model = std::make_unique<Model>(); // replace so its only data?
		std::unique_ptr<Material> material = std::make_unique<Material>(); // replace so its only data?
	};

}