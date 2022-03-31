#include <cmath>

#include "../../include/geometry/point.hpp"
#include "../../include/scenery/player.hpp"

point2d<float> player::try_move(int move_type, float t) const {
    if (!movement_scalar[move_type]) {
        return p;
    }
    return p + point2d<float>{std::cos(angle), std::sin(angle)} *
                   movement_scalar[move_type] * (t * s);
}

void player::rotate_camera(float c, float t) {
    angle += c * t * w;
}

void player::set_position(const point2d<float> &pt) { p = pt; }

float player::get_angle() const {
    return angle;
}

point2d<float> player::get_position() const { return p; }

void player::render(sdl::renderer_handle renderer,
                 sdl::texture_handle dot_texture) const {
    renderer.render(dot_texture, sdl::frect{p.x - 5, p.y - 5, 10, 10});
}