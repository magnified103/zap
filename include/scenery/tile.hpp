#ifndef SCENERY_TILE_HPP
#define SCENERY_TILE_HPP

#include "block.hpp"
#include "cereal/cereal.hpp"
#include "geometry/vector.hpp"

struct tile {
    int top_mesh_index;
    int middle_mesh_index;
    int bottom_mesh_index;
    float floor_level;
    float ceil_level;
    float lightning;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(top_mesh_index), CEREAL_NVP(middle_mesh_index),
                CEREAL_NVP(bottom_mesh_index), CEREAL_NVP(floor_level), CEREAL_NVP(ceil_level),
                CEREAL_NVP(lightning));
    }
};

#endif // SCENERY_TILE_HPP