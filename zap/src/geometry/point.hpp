#ifndef GEOMETRY_POINT_HPP
#define GEOMETRY_POINT_HPP

#include <cmath>
#include <type_traits>

template <class value_type>
struct vec2d;

template <class value_type>
using point2d = vec2d<value_type>;

template <class value_type>
struct vec2d {
    static_assert(std::is_floating_point_v<value_type>);

    vec2d operator+(const vec2d &v) const { return vec2d{x + v.x, y + v.y}; }

    vec2d operator-(const vec2d &v) const { return vec2d{x - v.x, y - v.y}; }

    vec2d operator*(value_type c) const { return vec2d{x * c, y * c}; }

    value_type abs() const { return std::sqrt(x * x + y * y); }

    value_type x, y;
};

#endif // GEOMETRY_POINT_HPP