#ifndef ENTITY_MONSTER_HPP
#define ENTITY_MONSTER_HPP

#include <cmath>

#include "cereal/cereal.hpp"
#include "entity/entity.hpp"
#include "geometry/vector.hpp"

struct monster : entity {
    int front_id;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(cereal::make_nvp("entity", (cereal::base_class<entity>(this))),
                CEREAL_NVP(front_id));
    }
};

#endif // ENTITY_MONSTER_HPP