#ifndef GEOMETRY_SEGMENT_HPP
#define GEOMETRY_SEGMENT_HPP

#include <optional>

#include "point.hpp"
#include "utilities.hpp"

struct segment_tag {};
struct ray_tag {};

template <class value_type, class tag>
class basic_segment;

template <class value_type>
using ray = basic_segment<value_type, ray_tag>;

template <class value_type>
using segment = basic_segment<value_type, segment_tag>;

template <class value_type, class tag>
struct basic_segment {
    point2d<value_type> a, b;
};

template <class value_type>
inline std::optional<point2d<value_type>>
get_intersection(const ray<value_type> &ray,
                 const segment<value_type> &segment) {
    // RAY in parametric: Point + Delta*T1
    value_type r_px = ray.a.x;
    value_type r_py = ray.a.y;
    value_type r_dx = ray.b.x - ray.a.x;
    value_type r_dy = ray.b.y - ray.a.y;

    // SEGMENT in parametric: Point + Delta*T2
    value_type s_px = segment.a.x;
    value_type s_py = segment.a.y;
    value_type s_dx = segment.b.x - segment.a.x;
    value_type s_dy = segment.b.y - segment.a.y;

    // Are they parallel? If so, no intersect
    value_type r_mag = std::sqrt(r_dx * r_dx + r_dy * r_dy);
    value_type s_mag = std::sqrt(s_dx * s_dx + s_dy * s_dy);
    if (approximately_equal(r_dx / r_mag, s_dx / s_mag) && approximately_equal(r_dy / r_mag, s_dy / s_mag)) {
        // Unit vectors are the same.
        return std::nullopt;
    }
    // SOLVE FOR T1 & T2
    // r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2
    // ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy
    // ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx -
    // r_py*r_dx
    // ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx)
    value_type T2 = (r_dx * (s_py - r_py) + r_dy * (r_px - s_px)) /
                    (s_dx * r_dy - s_dy * r_dx);
    value_type T1 = (s_px + s_dx * T2 - r_px) / r_dx;

    // Must be within parametic whatevers for RAY/SEGMENT
    if (T1 < 0) {
        return std::nullopt;
    }
    if (T2 < 0 || T2 > 1) {
        return std::nullopt;
    }

    // Return the POINT OF INTERSECTION
    return point2d<value_type>{r_px + r_dx * T1, r_py + r_dy * T1};
}

#endif // GEOMETRY_SEGMENT_HPP