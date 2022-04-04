#ifndef SCENERY_CAMERA_HPP
#define SCENERY_CAMERA_HPP

#include <algorithm>
#include <cmath>
#include <iostream>

#include "../wrapper/SDL.hpp"
#include "map.hpp"
#include "player.hpp"

class camera {
public:
    camera();

    camera(float fov);

    float get_horizontal_angle() const { return horizontal_angle; }

    float get_vertical_angle() const { return vertical_angle; }

    void rotate(float horizontal_changes, float vertical_changes);

    void render(sdl::renderer_handle renderer, player &player, map_grid &grid,
                float width, float height);

    void render_hits(sdl::renderer_handle renderer,
                     std::vector<std::pair<float, sdl::color>> hits,
                     float delta, float horizon, float base, float base_color);

    void render3d(sdl::renderer_handle renderer, player &player, map_grid &grid,
                  float width, float height);

private:
    float fov;
    float horizontal_angle;
    float vertical_angle;
};

#endif // SCENERY_CAMERA_HPP