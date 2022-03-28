#ifndef WRAPPER_GEOMETRY_HPP
#define WRAPPER_GEOMETRY_HPP

#include <type_traits>

#include <SDL2/SDL.h>

namespace sdl {

template <class value_type>
class basic_rect;

using rect = basic_rect<int>;
using frect = basic_rect<float>;

template <class value_type>
class basic_rect {
public:
    constexpr static bool is_integral = std::is_integral_v<value_type>;
    using rect_type = std::conditional_t<is_integral, SDL_Rect, SDL_FRect>;

    basic_rect() : rect{} {}

    basic_rect(value_type x, value_type y, value_type w, value_type h)
        : rect{x, y, w, h} {}

    rect_type get() noexcept { return rect; }

    const rect_type get() const noexcept { return rect; }

    rect_type *get_ptr() noexcept { return &rect; }

    const rect_type *get_ptr() const noexcept { return &rect; }

private:
    rect_type rect;
};

}

#endif // WRAPPER_GEOMETRY_HPP