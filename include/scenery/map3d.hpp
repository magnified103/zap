#ifndef SCENERY_MAP3D_HPP
#define SCENERY_MAP3D_HPP

#include <string>
#include <vector>

#include "block.hpp"
#include "cereal/cereal.hpp"
#include "opengl/gl.hpp"
#include "texture.hpp"
#include "tile.hpp"

struct map3d {
    int number_of_columns;
    int number_of_rows;
    float cell_size;
    std::vector<std::vector<tile>> terrain;
    std::vector<block> blocks;
    std::vector<texture> textures;
    std::vector<std::string> surfaces;

    // cereal specific
    template <class Archive>
    void serialize(Archive &archive) {
        archive(CEREAL_NVP(number_of_columns), CEREAL_NVP(number_of_rows),
                CEREAL_NVP(cell_size), CEREAL_NVP(terrain), CEREAL_NVP(blocks),
                CEREAL_NVP(textures), CEREAL_NVP(surfaces));
    }
};

void load_map(const std::string &path, map3d &map);

void save_map(const std::string &path, const map3d &map);

void load_textures(const std::string &path, const map3d &map,
                   std::vector<GLuint> &surface_ids);

void load_map_and_textures(const std::string &path, map3d &map,
                           std::vector<GLuint> &surface_ids);

#endif // SCENERY_MAP3D_HPP