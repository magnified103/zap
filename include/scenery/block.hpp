#ifndef SCENERY_BLOCK_HPP
#define SCENERY_BLOCK_HPP

#include <vector>

#include "cereal/cereal.hpp"
#include "geometry/vector.hpp"

struct block {
    int front_id;
    int left_id;
    int back_id;
    int right_id;
    int top_id;
    int bottom_id;
    float height_top;
    float height_bottom;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(front_id), CEREAL_NVP(left_id), CEREAL_NVP(back_id),
                CEREAL_NVP(right_id), CEREAL_NVP(top_id), CEREAL_NVP(bottom_id),
                CEREAL_NVP(height_top), CEREAL_NVP(height_bottom));
    }
};

#endif // SCENERY_BLOCK_HPP