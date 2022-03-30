#ifndef SCENERY_DOT_HPP
#define SCENERY_DOT_HPP

#include <cmath>

#include "../geometry/point.hpp"
#include "../wrapper/SDL.hpp"

struct movement_type {
    inline constexpr static int none = 0;
    inline constexpr static int up = 1;
    inline constexpr static int down = 2;
    inline constexpr static int left = 3;
    inline constexpr static int right = 4;
    inline constexpr static int upleft = 5;
    inline constexpr static int upright = 6;
    inline constexpr static int downleft = 7;
    inline constexpr static int downright = 8;
};

inline constexpr vec2d<float> movement_vectors[9] = {{0, 0},  {0, -1}, {0, 1},
                                                     {-1, 0}, {1, 0},  {-1, -1},
                                                     {1, -1}, {-1, 1}, {1, 1}};

inline float movement_vector_lengths[9] = {
    0, 1, 1, 1, 1, std::sqrt(2), std::sqrt(2), std::sqrt(2), std::sqrt(2)};

class dot {
public:
    dot(point2d<float> p, float s) : p(p), s(s) {}
    point2d<float> try_move(int move_type, float t) const;

    void set_position(const point2d<float> &pt);
    point2d<float> get_position() const;

    void render(sdl::renderer_handle renderer, sdl::texture_handle dot_texture) const;

private:
    point2d<float> p;
    float s;
};

#endif // SCENERY_DOT_HPP