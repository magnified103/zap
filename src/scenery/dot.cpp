#include <cmath>

#include "../../include/geometry/point.hpp"
#include "../../include/scenery/dot.hpp"

point2d<float> dot::try_move(int move_type, float t) const {
    if (!movement_vector_lengths[move_type]) {
        return p;
    }
    return p + movement_vectors[move_type] *
                   (t * s / movement_vector_lengths[move_type]);
}

void dot::set_position(const point2d<float> &pt) { p = pt; }

point2d<float> dot::get_position() const { return p; }

void dot::render(sdl::renderer_handle renderer,
                 sdl::texture_handle dot_texture) const {
    renderer.render(dot_texture, sdl::frect{p.x - 5, p.y - 5, 10, 10});
}