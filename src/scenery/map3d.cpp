#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#include "cereal/archives/json.hpp"
#include "cereal/types/string.hpp"
#include "cereal/types/vector.hpp"
#include "opengl/utilities.hpp"
#include "scenery/map3d.hpp"
#include "wrapper/exception.hpp"

void load_map(const std::string &path, map3d &map) {
    // read json file
    std::ifstream input_json(path);

    // test its existence
    if (!input_json) {
        throw std::runtime_error("Map not found");
    }

    // create deserializer
    cereal::JSONInputArchive input_archive(input_json);

    // black magic
    input_archive(CEREAL_NVP(map));
}

void save_map(const std::string &path, const map3d &map) {
    std::ofstream output_json(path);

    // create serializer
    cereal::JSONOutputArchive output_archive(output_json);

    // black magic again
    output_archive(CEREAL_NVP(map));
}

void load_textures(const std::string &path, const map3d &map,
                   std::vector<GLuint> &surface_ids) {
    surface_ids.clear();

    for (const auto &surface_path : map.surfaces) {
        // get surface's absolute path
        std::string absolute_path(std::filesystem::path(path).parent_path() /
                                  std::filesystem::path(surface_path));
        surface_ids.push_back(load_texture(absolute_path));
    }
}

void load_map_and_textures(const std::string &path, map3d &map,
                           std::vector<GLuint> &surface_ids) {
    load_map(path, map);

    load_textures(path, map, surface_ids);
}