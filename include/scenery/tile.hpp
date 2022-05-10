#ifndef SCENERY_TILE_HPP
#define SCENERY_TILE_HPP

#include "block.hpp"
#include "cereal/cereal.hpp"
#include "geometry/vector.hpp"

struct tile {
    int top_id;
    int middle_id;
    int bottom_id;
    vec3 point_a;
    vec3 point_b;
    float lightning;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(top_id), CEREAL_NVP(middle_id),
                CEREAL_NVP(bottom_id), CEREAL_NVP(point_a), CEREAL_NVP(point_b),
                CEREAL_NVP(lightning));
    }
};

#endif // SCENERY_TILE_HPP