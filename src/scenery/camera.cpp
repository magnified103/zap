#include <algorithm>
#include <cmath>

#include "../../include/scenery/camera.hpp"
#include "../../include/scenery/player.hpp"
#include "../../include/scenery/map.hpp"
#include "../../include/wrapper/SDL.hpp"

camera::camera() : fov(std::acos(-1) / 3) {}
camera::camera(float fov) : fov(fov) {}
void camera::render(sdl::renderer_handle renderer, player &player, map_grid &grid,
                    float width, float height) {
    int ray_count = 300;
    float angle = player.get_angle();
    float base_angle = angle - fov / 2;
    float base_w = fov / ray_count;
    std::vector<float> hits;
    point2d<float> player_position = player.get_position();
    // sdl::log_info("%.2f %.2f", player_position.x, player_position.y);
    for (int i = 0; i < ray_count; i++) {
        point2d<float> delta{std::cos(base_angle), std::sin(base_angle)};
        hits.push_back(
            grid.hit_distance(player_position,
                              {player_position, player_position + delta}) *
            std::cos(base_angle - angle));
        // sdl::log_info("%.2f %.2f %.2f", base_w, delta.x, delta.y);
        base_angle += base_w;
    }
    render_hits(renderer, hits, width / ray_count, height / 2, 20000, 200);
}
void camera::render_hits(sdl::renderer_handle renderer, std::vector<float> hits,
                         float delta, float horizon, float base_height,
                         float base_color) {
    float x = 0;
    for (auto hit : hits) {
        float wall_height = base_height / hit;
        Uint8 brightness = 0xff - std::min(1.0f, hit / base_color) * 0xff;
        renderer.set_draw_color(sdl::color{brightness, brightness, brightness});
        renderer.fill_rect(
            sdl::frect{x, horizon - wall_height / 2, delta, wall_height});
        x += delta;
    }
}