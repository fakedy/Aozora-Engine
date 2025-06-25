#pragma once
#include <entt/entt.hpp>


namespace Aozora {

    struct RelationComponent {
        entt::entity parent{ entt::null };
        entt::entity firstChild{ entt::null };
        entt::entity nextSibling{ entt::null };
    };

}