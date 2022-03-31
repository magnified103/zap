#ifndef SCENERY_DOT_HPP
#define SCENERY_DOT_HPP

#include <cmath>

#include "../geometry/point.hpp"
#include "../wrapper/SDL.hpp"

struct movement_type {
    inline constexpr static int none = 0;
    inline constexpr static int up = 1;
    inline constexpr static int down = 2;
};

inline constexpr float movement_scalar[3] = {0,  1, -1};

class player {
public:
    player(point2d<float> p, float angle, float s, float w) : p(p), angle(angle), s(s), w(w) {}
    point2d<float> try_move(int move_type, float t) const;

    void rotate_camera(float c, float t);

    void set_position(const point2d<float> &pt);
    point2d<float> get_position() const;
    float get_angle() const;

    void render(sdl::renderer_handle renderer, sdl::texture_handle dot_texture) const;

private:
    point2d<float> p;
    float angle;
    float s;
    float w;
};

#endif // SCENERY_DOT_HPP