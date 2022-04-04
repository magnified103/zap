#ifndef SCENERY_WALL_HPP
#define SCENERY_WALL_HPP

#include "../geometry/vector.hpp"
#include "../wrapper/SDL.hpp"

struct wall {
    vec2 endpoints[2];
    sdl::color color;
    vec2 get_centroid() const { return (endpoints[0] + endpoints[1]) * 0.5f; }
};

#endif // SCENERY_WALL_HPP