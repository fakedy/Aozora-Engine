#pragma once
#include <memory>
#include "Systems/Model.h"

namespace Aozora {

	struct ModelComponent {
		std::unique_ptr<Model> model = std::make_unique<Model>();
	};

}