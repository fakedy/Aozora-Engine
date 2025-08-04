#pragma once
#include <entt/entt.hpp>
#include <cereal/cereal.hpp>
#include <cereal/types/vector.hpp>

namespace Aozora {

    struct RelationComponent {
    public:
        entt::entity parent{ entt::null };
        entt::entity firstChild{ entt::null };
        std::vector<entt::entity> children;
        entt::entity nextSibling{ entt::null };


        template<class Archive>
        void serialize(Archive& archive) {
            archive(CEREAL_NVP(parent),
                CEREAL_NVP(firstChild),
                CEREAL_NVP(children),
                CEREAL_NVP(nextSibling));
        }


    };

}