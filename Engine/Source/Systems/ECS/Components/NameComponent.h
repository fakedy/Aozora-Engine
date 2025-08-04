#pragma once
#include <string>
#include <cereal/cereal.hpp>
#include <cereal/types/string.hpp>

namespace Aozora {

	struct NameComponent {
	public:
		std::string name{};


		template<class Archive>
		void serialize(Archive& archive) {
			archive(CEREAL_NVP(name));
		}


	};

}