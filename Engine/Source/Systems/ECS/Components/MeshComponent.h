#pragma once
#include <vector>
#include <glm/glm.hpp>
#include <string>


namespace Aozora {

	struct MeshComponent {

		bool m_isActive{ true };
		std::vector<unsigned int> meshIDs{0};
		unsigned int material{ 0 };
	};

}