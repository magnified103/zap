#ifndef SCENERY_TEXTURE_HPP
#define SCENERY_TEXTURE_HPP

#include <vector>

#include <cereal/cereal.hpp>

#include "geometry/vector.hpp"

struct texture {
    int surface_id;
    std::vector<vec3> positions;
    std::vector<vec2> UVs;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(surface_id), CEREAL_NVP(positions), CEREAL_NVP(UVs));
    }
};

#endif // SCENERY_TEXTURE_HPP