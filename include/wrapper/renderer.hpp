#ifndef WRAPPER_RENDERER_HPP
#define WRAPPER_RENDERER_HPP

#include <SDL2/SDL.h>

#include "color.hpp"
#include "common.hpp"
#include "geometry.hpp"

namespace sdl {

template <class traits>
class basic_renderer;

using renderer = basic_renderer<detail::owner_tag>;
using renderer_handle = basic_renderer<detail::handle_tag>;

template <class traits>
class basic_renderer {
public:
    basic_renderer(SDL_Renderer *renderer) : renderer{renderer} {
        if constexpr (detail::is_owner<traits>) {
            if (!renderer) {
                throw exception("Cannot create renderer from nullptr");
            }
        }
    }

    template <class T = traits, detail::enable_for_handle<T> = 0>
    basic_renderer(const renderer &other) : renderer{other.get()} {}

    ~basic_renderer() {
        if constexpr (detail::is_owner<traits>) {
            SDL_DestroyRenderer(renderer);
        }
    }

    SDL_Renderer *get() const noexcept { return renderer; }

    int clear() { return SDL_RenderClear(renderer); }

    void present() { SDL_RenderPresent(renderer); }

    template <class value_type>
    int draw_rect(basic_rect<value_type> rect) {
        if constexpr (rect.is_integral) {
            return SDL_RenderDrawRect(renderer, rect.get_ptr());
        } else {
            return SDL_RenderDrawRectF(renderer, rect.get_ptr());
        }
    }

    template <class value_type>
    int fill_rect(basic_rect<value_type> rect) {
        if constexpr (rect.is_integral) {
            return SDL_RenderFillRect(renderer, rect.get_ptr());
        } else {
            return SDL_RenderFillRectF(renderer, rect.get_ptr());
        }
    }

    int set_draw_color(color color) {
        return SDL_SetRenderDrawColor(renderer, color.red(), color.green(),
                                      color.blue(), color.alpha());
    }

private:
    SDL_Renderer *renderer;
};

}

#endif // WRAPPER_RENDERER_HPP