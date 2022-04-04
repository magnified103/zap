#ifndef SCENERY_PLAYER_HPP
#define SCENERY_PLAYER_HPP

#include <cmath>

#include "../geometry/point.hpp"
#include "../wrapper/SDL.hpp"
#include "camera.hpp"

struct movement_type {
    inline constexpr static int none = 0;
    inline constexpr static int up = 1;
    inline constexpr static int down = 2;
};

inline constexpr float movement_scalar[3] = {0, 1, -1};

class camera;

class player {
public:
    player(point2d<float> p, float horizontal_speed)
        : p(p), horizontal_speed(horizontal_speed) {}

    point2d<float> try_move(camera &camera, int move_type, float t) const;

    void set_position(const point2d<float> &pt);

    point2d<float> get_position() const;

    void render(sdl::renderer_handle renderer,
                sdl::texture_handle dot_texture) const;

private:
    point2d<float> p;
    float horizontal_speed;
};

#endif // SCENERY_PLAYER_HPP