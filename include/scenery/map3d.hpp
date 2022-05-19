#ifndef SCENERY_MAP3D_HPP
#define SCENERY_MAP3D_HPP

#include <cmath>
#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "entity/entity.hpp"
#include "entity/monster.hpp"
#include "entity/projectile.hpp"
#include "opengl/gl.hpp"
#include "scenery/block.hpp"
#include "scenery/texture.hpp"
#include "scenery/tile.hpp"

struct map3d {
    int number_of_columns;
    int number_of_rows;
    float cell_size;
    std::vector<std::vector<tile>> terrain;
    std::vector<block> blocks;
    std::vector<monster> monsters;
    std::vector<projectile_type> projectile_types;
    std::vector<texture> textures;
    std::vector<std::string> surfaces;

    // runtime specific
    std::vector<projectile> projectiles;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(number_of_columns), CEREAL_NVP(number_of_rows), CEREAL_NVP(cell_size),
                CEREAL_NVP(terrain), CEREAL_NVP(blocks), CEREAL_NVP(monsters),
                CEREAL_NVP(projectile_types), CEREAL_NVP(textures), CEREAL_NVP(surfaces));
    }
    bool collide_floor(const vec3 &position, float margin = 0) {
        int column_index = std::floor(position.x / cell_size);
        int row_index = std::floor(position.z / cell_size);

        // out of bound?
        if (column_index < 0 || column_index >= number_of_columns || row_index < 0 ||
            row_index >= number_of_rows) {
            return true;
        }
        return (position.y + margin < terrain[column_index][row_index].floor_level);
    }
    bool collide_ceil(const vec3 &position) {
        int column_index = std::floor(position.x / cell_size);
        int row_index = std::floor(position.z / cell_size);

        // out of bound?
        if (column_index < 0 || column_index >= number_of_columns || row_index < 0 ||
            row_index >= number_of_rows) {
            return true;
        }
        return (position.y > terrain[column_index][row_index].ceil_level);
    }
};

#endif // SCENERY_MAP3D_HPP