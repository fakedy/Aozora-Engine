#pragma once
#include <entt/entt.hpp>


namespace Aozora {

    struct RelationComponent {
        entt::entity parent{ entt::null };
        entt::entity firstChild{ entt::null };
        std::vector<entt::entity> children;
        entt::entity nextSibling{ entt::null };
    };

}