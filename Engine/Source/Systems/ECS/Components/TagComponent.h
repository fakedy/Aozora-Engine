#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>
namespace Aozora {

	struct TagComponent {
	public:
		std::string tag{};

		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(tag));
		}
	};
}
