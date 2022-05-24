#ifndef SDL2_GEOMETRY_HPP
#define SDL2_GEOMETRY_HPP

#include <type_traits>

#include <SDL.h>

namespace sdl {

template <class value_type>
class basic_point;

using point = basic_point<int>;
using fpoint = basic_point<float>;

template <class value_type>
class basic_point {
public:
    constexpr static bool is_integral = std::is_integral_v<value_type>;
    using point_type = std::conditional_t<is_integral, SDL_Point, SDL_FPoint>;

    basic_point() : sdl_point{} {}

    basic_point(value_type x, value_type y) : sdl_point{x, y} {}

    point_type get() noexcept { return sdl_point; }

    const point_type &get() const noexcept { return sdl_point; }

    point_type *get_ptr() noexcept { return &sdl_point; }

    const point_type *get_ptr() const noexcept { return &sdl_point; }

private:
    point_type sdl_point;
};

template <class value_type>
class basic_rect;

using rect = basic_rect<int>;
using frect = basic_rect<float>;

template <class value_type>
class basic_rect {
public:
    constexpr static bool is_integral = std::is_integral_v<value_type>;
    using rect_type = std::conditional_t<is_integral, SDL_Rect, SDL_FRect>;

    basic_rect() : sdl_rect{} {}

    basic_rect(value_type x, value_type y, value_type w, value_type h)
        : sdl_rect{x, y, w, h} {}

    rect_type get() noexcept { return sdl_rect; }

    const rect_type &get() const noexcept { return sdl_rect; }

    rect_type *get_ptr() noexcept { return &sdl_rect; }

    const rect_type *get_ptr() const noexcept { return &sdl_rect; }

private:
    rect_type sdl_rect;
};

} // namespace sdl

#endif // SDL2_GEOMETRY_HPP