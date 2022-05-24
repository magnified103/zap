#ifndef SCENERY_MESH_HPP
#define SCENERY_MESH_HPP

#include <array>
#include <vector>

#include <cereal/cereal.hpp>

#include "geometry/vector.hpp"
#include "opengl/gl.hpp"

struct triangle {
    std::array<int, 3> vertex_indices;
    std::array<vec2, 3> UVs;
    int texture_index;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(vertex_indices), CEREAL_NVP(UVs), CEREAL_NVP(texture_index));
    }
};

struct mesh {
    std::vector<vec3> vertices;
    std::vector<triangle> faces;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(vertices), CEREAL_NVP(faces));
    }
};

#endif // SCENERY_MESH_HPP