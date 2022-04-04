#include <cmath>

#include "../../include/geometry/point.hpp"
#include "../../include/scenery/camera.hpp"
#include "../../include/scenery/player.hpp"

point2d<float> player::try_move(camera &camera, int move_type, float t) const {
    if (!movement_scalar[move_type]) {
        return p;
    }
    float angle = camera.get_horizontal_angle();
    return p + point2d<float>{std::cos(angle), std::sin(angle)} *
                   movement_scalar[move_type] * (t * horizontal_speed);
}

void player::set_position(const point2d<float> &pt) { p = pt; }

point2d<float> player::get_position() const { return p; }

void player::render(sdl::renderer_handle renderer,
                    sdl::texture_handle dot_texture) const {
    renderer.render(dot_texture, sdl::frect{p.x - 5, p.y - 5, 10, 10});
}