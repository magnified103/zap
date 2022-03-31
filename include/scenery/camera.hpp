#ifndef SCENERY_CAMERA_HPP
#define SCENERY_CAMERA_HPP

#include <algorithm>
#include <cmath>
#include <iostream>

#include "../wrapper/SDL.hpp"
#include "player.hpp"
#include "map.hpp"

class camera {
public:
    camera();
    camera(float fov);
    void render(sdl::renderer_handle renderer, player &player, map_grid &grid,
                float width, float height);
    void render_hits(sdl::renderer_handle renderer, std::vector<float> hits,
                float delta, float horizon, float base, float base_color);

private:
    float fov;
};

#endif // SCENERY_CAMERA_HPP