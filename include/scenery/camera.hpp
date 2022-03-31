#ifndef SCENERY_CAMERA_HPP
#define SCENERY_CAMERA_HPP

#include <cmath>
#include <iostream>
#include <algorithm>

#include "../wrapper/SDL.hpp"
#include "dot.hpp"
#include "map.hpp"

class camera {
public:
    camera() : fov(std::acos(-1) / 3) {}
    camera(float fov) : fov(fov) {}
    void render(sdl::surface_handle surface, dot &player, map_grid &grid,
                float width, float height) {
        int ray_count = 200;
        float base_angle = player.get_angle() - fov / 2;
        float base_w = fov / ray_count;
        std::vector<float> hits;
        point2d<float> player_position = player.get_position();
        // sdl::log_info("%.2f %.2f", player_position.x, player_position.y);
        for (int i = 0; i < ray_count; i++) {
            point2d<float> delta{std::cos(base_angle), std::sin(base_angle)};
            hits.push_back(grid.hit_distance(
                player_position, {player_position, player_position + delta}));
            // sdl::log_info("%.2f %.2f %.2f", base_w, delta.x, delta.y);
            base_angle += base_w;
        }
        // for (auto hit : hits) {
        //     std::cerr << hit << " ";
        // }
        // std::cerr << "\n";
        render(surface, hits, width / ray_count, height / 2, 15000, 50);
    }
    void render(sdl::surface_handle surface, std::vector<float> hits,
                float delta, float horizon, float base, float base_color) {
        float x = 0;
        for (auto hit : hits) {
            float wall_height = base / hit;
            int brightness = std::min(255.0f, 0xff / ((hit + 1) / base_color));
            surface.fill_rect(
                sdl::rect{x, horizon - wall_height / 2, delta, wall_height},
                sdl::color{brightness, brightness, brightness});
            x += delta;
        }
    }

private:
    float fov;
};

#endif // SCENERY_CAMERA_HPP