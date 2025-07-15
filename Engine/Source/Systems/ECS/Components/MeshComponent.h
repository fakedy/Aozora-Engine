#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>


namespace Aozora {

	struct MeshComponent {

		bool m_isActive{ true };
		uint32_t meshID{0};
		uint32_t materialID{ 0 };
	};

}