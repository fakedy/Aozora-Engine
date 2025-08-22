#pragma once
#include <stdint.h>

namespace Aozora {

	struct ModelComponent {
	public:
		uint64_t ID;

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(ID));
		}
	};
}