#ifndef SCENERY_MAP_UTIL_HPP
#define SCENERY_MAP_UTIL_HPP

#include <string>
#include <vector>

#include "cereal/cereal.hpp"
#include "entity/player3d.hpp"
#include "opengl/gl.hpp"
#include "physics/physics.hpp"
#include "scenery/map3d.hpp"

void load_map(const std::string &path, map3d &map, player3d &player, physics &engine);

void save_map(const std::string &path, const map3d &map, const player3d &player,
              const physics &engine);

void load_textures(const std::string &path, const map3d &map, std::vector<GLuint> &surface_ids);

void load_map_and_textures(const std::string &path, map3d &map, player3d &player, physics &engine,
                           std::vector<GLuint> &surface_ids);

#endif // SCENERY_MAP_UTIL_HPP